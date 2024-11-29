#pragma once
#include <string>
#include <vector>
#include "connection_pool.h"
#include "mysql_conn.h"
#include "client/rpc_client.h"
#include "midinfo.h"

namespace monitor {
class QueryData {
   public:
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    bool queryDataInfo(std::string account_num, int count,
                       monitor::RpcClient& rpc_client);


    std::vector<MidInfo> query_result_array_;
};
}  // namespace monitor