#pragma once
#include <string>
#include <vector>
#include "ConnectionPool.h"
#include "MysqlConn.h"
#include "midinfo.h"
namespace monitor {
class History {
   public:
ConnectionPool* pool = ConnectionPool::getConnectPool();
    std::vector<MidInfo> getHistoryInfo(std::string account_num, int count);
    std::string SelectUserId(std::string accountNum);
};
}  // namespace monitor