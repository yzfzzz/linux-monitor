#pragma once
#include <string>
#include <vector>
#include "client/rpc_client.h"
#include "connection_pool.h"
#include "midinfo.h"
#include "mysql_conn.h"

namespace monitor {
class QueryData {
   public:
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    bool queryDataInfo(std::string account_num, std::string machine_name,
                       int count, monitor::RpcClient& rpc_client);

    std::vector<MidInfo> query_result_array_;
};
}  // namespace monitor