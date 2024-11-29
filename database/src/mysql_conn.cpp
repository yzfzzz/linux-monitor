#include "mysql_conn.h"

MysqlConn::MysqlConn() {
    conn_ = mysql_init(nullptr);
    mysql_set_character_set(conn_, "utf8");
}

MysqlConn::~MysqlConn() {
    if (conn_ != nullptr) {
        mysql_close(conn_);
    }
    freeResult();
}

bool MysqlConn::connect(std::string user, std::string passwd,
                        std::string database_name, std::string ip,
                        unsigned short port) {
    MYSQL* mysql_ptr =
        mysql_real_connect(conn_, ip.c_str(), user.c_str(), passwd.c_str(),
                           database_name.c_str(), port, nullptr, 0);
    if (mysql_ptr) {
        return true;
    } else {
        return false;
    }
}

bool MysqlConn::update(std::string sql) {
    if (mysql_query(conn_, sql.c_str())) {
        return false;
    }
    return true;
}

bool MysqlConn::query(std::string sql) {
    freeResult();
    if (mysql_query(conn_, sql.c_str())) {
        return false;
    }

    result_ = mysql_store_result(conn_);
    return true;
}

bool MysqlConn::next() {
    if (result_ != nullptr) {
        row_ = mysql_fetch_row(result_);
        if (row_ != nullptr) return true;
    }
    return false;
}

// 对char* row[]按索引(index)取值, 若index无效, 则返回空字符串
std::string MysqlConn::value(int index) {
    int colCount = mysql_num_fields(result_);
    if (index >= colCount || index < 0) {
        return std::string();
    }
    char* val = row_[index];
    unsigned long length = mysql_fetch_lengths(result_)[index];
    return std::string(val, length);
}

bool MysqlConn::transaction() { return mysql_autocommit(conn_, false); }

bool MysqlConn::commit() { return mysql_commit(conn_); }

bool MysqlConn::rollback() { return mysql_rollback(conn_); }

void MysqlConn::freeResult() {
    if (result_) {
        mysql_free_result(result_);
        result_ = nullptr;
    }
}

void MysqlConn::refreshAliveTime() {
    alive_time_ = std::chrono::steady_clock::now();
}

long long MysqlConn::getAliveTime() {
    std::chrono::nanoseconds interval_time =
        std::chrono::steady_clock::now() - alive_time_;
    std::chrono::microseconds millsec =
        std::chrono::duration_cast<std::chrono::microseconds>(interval_time);
    return millsec.count();
}