#pragma once
#include <string>
#include <vector>
#include "ConnectionPool.h"
#include "MysqlConn.h"
#include "midinfo.h"
namespace monitor {
class queryData {
   public:
ConnectionPool* pool = ConnectionPool::getConnectPool();
    bool queryDataInfo(std::string account_num, int count);
    std::string SelectUserId(std::string accountNum);

    std::vector<MidInfo> queryData_array;
};
}  // namespace monitor