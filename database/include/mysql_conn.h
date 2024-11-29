#pragma once
#include <mysql/mysql.h>
#include <chrono>
#include <iostream>
#include <string>

class MysqlConn {
   public:
    // 初始化数据库连接
    MysqlConn();
    // 释放数据库连接
    ~MysqlConn();
    // 连接数据库
    bool connect(std::string user, std::string passwd,
                 std::string database_name, std::string ip,
                 unsigned short port = 3306);
    // 更新数据库, insert/update/delete
    bool update(std::string sql);
    // 查询数据库
    bool query(std::string sql);
    // 遍历查询得到结果集
    bool next();
    // 得到结果集中的字段值
    std::string value(int index);
    // 开启事务操作(手动提交)
    bool transaction();
    // 提交事务
    bool commit();
    // 事务回滚
    bool rollback();
    // 刷新上一次活动时间
    void refreshAliveTime();
    // 计算空闲的总时长
    long long getAliveTime();

   private:
    // 释放结果集空间
    void freeResult();
    MYSQL* conn_ = nullptr;
    MYSQL_RES* result_ = nullptr;
    MYSQL_ROW row_ = nullptr;
    // 当前连接上一次活动时刻
    std::chrono::steady_clock::time_point alive_time_;
};