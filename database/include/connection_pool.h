#pragma once
#include <fcntl.h>
#include <mysql_conn.h>
#include <unistd.h>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
class ConnectionPool {
   public:
    static ConnectionPool* getConnectPool();
    ConnectionPool(const ConnectionPool& obj) = delete;
    ConnectionPool& operator=(const ConnectionPool& obj) = delete;

    std::shared_ptr<MysqlConn> getConnection();
    ~ConnectionPool();

   private:
    ConnectionPool();
    // 加载配置文件
    bool parseJsonFile();
    // 生产线程
    void produceConnection();
    // 回收线程
    void recycleConnection();
    // 连接池中新增连接
    void addConnection();

    std::string ip_;
    std::string user_;
    std::string passwd_;
    std::string database_name_;
    unsigned short port_;
    int min_size_;
    int max_size_;
    int timeout_;
    int max_idle_time_;
    std::queue<MysqlConn*> connection_queue;
    std::mutex mutex_;
    std::condition_variable cond_;
};
