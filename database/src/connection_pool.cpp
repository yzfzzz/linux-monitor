#include "connection_pool.h"

ConnectionPool* ConnectionPool::getConnectPool() {
    // 单例模式,一次创建, 终生存在, 直到进程死亡

    // 存储在静态数据区的变量会在程序刚开始运行时就完成初始化，
    // 也是唯一的一次初始化。之后再次运行到含有 static 关键字的
    // 初始化语句时不会再执行该语句

    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::parseJsonFile() {
    std::ifstream input_file("dbconf.json");
    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();

    ip_ = json_data["ip"];
    port_ = json_data["port"];
    user_ = json_data["userName"];
    passwd_ = json_data["password"];
    database_name_ = json_data["dbName"];
    min_size_ = json_data["minSize"];
    max_size_ = json_data["maxSize"];
    max_idle_time_ = json_data["maxIdleTime"];
    timeout_ = json_data["timeout"];
    return true;
}

ConnectionPool::ConnectionPool() {
    if (!parseJsonFile()) {
        return;
    }

    for (int i = 0; i < min_size_; i++) {
        addConnection();
    }

    // 传入this指针, 确定调用哪个(类)对象的成员函数
    std::thread producer_thread(&ConnectionPool::produceConnection, this);
    std::thread recycler_thread(&ConnectionPool::recycleConnection, this);

    producer_thread.detach();
    recycler_thread.detach();
    // 此时应有3个线程在工作
}

void ConnectionPool::addConnection() {
    MysqlConn* conn = new MysqlConn;
    conn->connect(user_, passwd_, database_name_, ip_, port_);
    connection_queue.push(conn);
}

void ConnectionPool::produceConnection() {
    while (true) {
        std::unique_lock<std::mutex> locker(mutex_);

        while (connection_queue.size() >= min_size_) {
            // 需要消耗一个conn,才会解除堵塞
            cond_.wait(locker);

            // 解除完堵塞, 继续判断是否缺conn,
            // 缺则跳出本次while执行下面的add语句
        }
        addConnection();
        cond_.notify_all();
    }
    // 跑到这里互斥锁释放了
}

void ConnectionPool::recycleConnection() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> locker(mutex_);
        while (connection_queue.size() > min_size_ ||
               connection_queue.size() > max_size_) {
            MysqlConn* conn = connection_queue.front();
            if (connection_queue.size() > max_size_) {
                connection_queue.pop();
                delete conn;
            } else if (connection_queue.size() > min_size_ &&
                       connection_queue.size() < max_size_ &&
                       conn->getAliveTime() >= max_idle_time_) {
                connection_queue.pop();
                delete conn;
            } else {
                break;
            }
        }
    }
}

std::shared_ptr<MysqlConn> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> locker(mutex_);
    while (connection_queue.empty()) {
        // 需要 cond_ 被唤醒才能跳出这个while循坏
        if (std::cv_status::timeout ==
            cond_.wait_for(locker, std::chrono::milliseconds(timeout_))) {
            if (connection_queue.empty()) {
                continue;
            }
        }
    }
    // 优先调用我们自定义的释放规则
    std::shared_ptr<MysqlConn> connptr(
        connection_queue.front(), [this](MysqlConn* conn) {
            std::lock_guard<std::mutex> locker(mutex_);
            conn->refreshAliveTime();
            connection_queue.push(conn);
        });
    connection_queue.pop();
    cond_.notify_all();
    return connptr;
}

ConnectionPool::~ConnectionPool() {
    while (!connection_queue.empty()) {
        MysqlConn* conn = connection_queue.front();
        connection_queue.pop();
        delete conn;
    }
}