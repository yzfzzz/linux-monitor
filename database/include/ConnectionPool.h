#pragma once
#include <MysqlConn.h>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>
class ConnectionPool {
   public:
    static ConnectionPool* getConnectPool();
    // ½ûÖ¹¿½±´¹¹Ôì
    ConnectionPool(const ConnectionPool& obj) = delete;
    ConnectionPool& operator=(const ConnectionPool& obj) = delete;

    std::shared_ptr<MysqlConn> getConnection();
    ~ConnectionPool();

   private:
    ConnectionPool();
    bool parseJsonFile();

    void produceConnection();
    void recycleConnection();
    void addConnection();

    std::string m_ip;
    std::string m_user;
    std::string m_passwd;
    std::string m_dbName;
    unsigned short m_port;
    int m_minSize;
    int m_maxSize;
    int m_timeout;
    int m_maxIdleTime;
    std::queue<MysqlConn*> m_connectionQ;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};
