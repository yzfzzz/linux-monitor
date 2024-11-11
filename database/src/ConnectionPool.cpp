#include "ConnectionPool.h"

ConnectionPool* ConnectionPool::getConnectPool() {
    // 一次创建, 终生存在, 直到进程死亡
    /*
    存储在静态数据区的变量会在程序刚开始运行时就完成初始化，
    也是唯一的一次初始化。之后再次运行到含有 static 关键字的
    初始化语句时不会再执行该语句
    */
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::parseJsonFile() {
    std::ifstream inFile("dbconf.json");
    nlohmann::json data;
    inFile >> data;
    inFile.close();

    m_ip = data["ip"];
    m_port = data["port"];
    m_user = data["userName"];
    m_passwd = data["password"];
    m_dbName = data["dbName"];
    m_minSize = data["minSize"];
    m_maxSize = data["maxSize"];
    m_maxIdleTime = data["maxIdleTime"];
    m_timeout = data["timeout"];
    return true;
}

ConnectionPool::ConnectionPool() {
    // 加载配置文件
    if (!parseJsonFile()) {
        return;
    }

    for (int i = 0; i < m_minSize; i++) {
        addConnection();
    }

    /* 为什么要传入this指针?
    确定调用哪个(类)对象的成员函数
    */
    std::thread producer(&ConnectionPool::produceConnection, this);
    std::thread recycler(&ConnectionPool::recycleConnection, this);

    producer.detach();
    recycler.detach();
}

void ConnectionPool::addConnection() {
    MysqlConn* conn = new MysqlConn;
    conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
    m_connectionQ.push(conn);
}

void ConnectionPool::produceConnection() {
    while (true) {
        std::unique_lock<std::mutex> locker(m_mutex);

        while (m_connectionQ.size() >= m_minSize) {
            // 需要消耗一个conn,才会解除堵塞
            m_cond.wait(locker);

            /* 解除完堵塞, 继续判断是否缺conn,
            缺则跳出本次while执行下面的add语句, 否则再一次进入堵塞
            */
        }
        addConnection();
        m_cond.notify_all();
    }  // 跑到这里互斥锁释放了
}

void ConnectionPool::recycleConnection() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> locker(m_mutex);
        while (m_connectionQ.size() > m_minSize ||
               m_connectionQ.size() > m_maxSize) {
            MysqlConn* conn = m_connectionQ.front();
            if (m_connectionQ.size() > m_maxSize) {
                m_connectionQ.pop();
                delete conn;
            } else if (m_connectionQ.size() > m_minSize &&
                       m_connectionQ.size() < m_maxSize &&
                       conn->getAliveTime() >= m_maxIdleTime) {
                m_connectionQ.pop();
                delete conn;
            } else {
                break;
            }
        }
    }
}

std::shared_ptr<MysqlConn> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_connectionQ.empty()) {
        // 需要 m_cond 被唤醒才能跳出这个while循坏
        if (std::cv_status::timeout ==
            m_cond.wait_for(locker, std::chrono::milliseconds(m_timeout))) {
            if (m_connectionQ.empty()) {
                continue;
            }
        }
    }
    // share_ptr的构造
    // 优先调用我们自定义的释放规则
    std::shared_ptr<MysqlConn> connptr(
        m_connectionQ.front(), [this](MysqlConn* conn) {
            std::lock_guard<std::mutex> locker(m_mutex);
            conn->refreshAliveTime();
            m_connectionQ.push(conn);
        });
    m_connectionQ.pop();
    m_cond.notify_all();
    return connptr;
}

ConnectionPool::~ConnectionPool() {
    while (!m_connectionQ.empty()) {
        MysqlConn* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}