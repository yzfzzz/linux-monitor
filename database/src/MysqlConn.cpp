#include "MysqlConn.h"

MysqlConn::MysqlConn() {
    m_conn = mysql_init(nullptr);
    mysql_set_character_set(m_conn, "utf8");
}

MysqlConn::~MysqlConn() {
    if (m_conn != nullptr) {
        mysql_close(m_conn);
    }
    freeResult();
}

bool MysqlConn::connect(std::string user, std::string passwd,
                        std::string dbName, std::string ip,
                        unsigned short port) {
    MYSQL* ptr =
        mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(),
                           dbName.c_str(), port, nullptr, 0);
    if (ptr) {
        // std::cout << "Mysql connect success!" << std::endl;
        return true;
    } else {
        // std::cout << "Mysql connect failed!" << std::endl;
        return false;
    }
}


bool MysqlConn::update(std::string sql) {
    if (mysql_query(m_conn, sql.c_str())) {
        return false;
    }
    return true;
}

// 负责发送sql语句
bool MysqlConn::query(std::string sql) {
    freeResult();
    if (mysql_query(m_conn, sql.c_str())) {
        return false;
    }
    // 存查询返回的结果, 可以理解为二维数组
    m_result = mysql_store_result(m_conn);
    return true;
}

bool MysqlConn::next() {
    if (m_result != nullptr) {
        // 按行遍历
        m_row = mysql_fetch_row(m_result);
        if (m_row != nullptr) return true;
    }
    return false;
}

std::string MysqlConn::value(int index) {
    // 按列遍历
    int colCount = mysql_num_fields(m_result);
    if (index >= colCount || index < 0) {
        // 越界, 返回null
        return std::string();
    }
    char* val = m_row[index];
    // 防止string中间遇到‘/0’截断
    unsigned long length = mysql_fetch_lengths(m_result)[index];
    return std::string(val, length);
}

bool MysqlConn::transaction() { return mysql_autocommit(m_conn, false); }

bool MysqlConn::commit() { return mysql_commit(m_conn); }
bool MysqlConn::rollback() { return mysql_rollback(m_conn); }

void MysqlConn::freeResult() {
    if (m_result) {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}

void MysqlConn::refreshAliveTime() {
    m_alivetime = std::chrono::steady_clock::now();
}

long long MysqlConn::getAliveTime() {
    std::chrono::nanoseconds res =
        std::chrono::steady_clock::now() - m_alivetime;
    std::chrono::microseconds millsec =
        std::chrono::duration_cast<std::chrono::microseconds>(res);
    return millsec.count();
}