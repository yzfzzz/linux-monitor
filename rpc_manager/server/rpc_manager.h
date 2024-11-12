#pragma once
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include "ConnectionPool.h"
#include "MysqlConn.h"
#include "monitor_info.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

namespace monitor {

class MidInfo {
    public:
    int gpu_num = 0;
    std::string gpu_name = "";
    int gpu_used_mem = -1;
    int gpu_total_mem = -1;
    int gpu_avg_util = -1;
    float cpu_load_avg_1 = -1.0;
    float cpu_load_avg_3 = -1.0;
    float cpu_load_avg_15 = -1.0;
    float mem_used = -1.0;
    float mem_total = -1.0;
    float net_send_rate = -1.0;
    float net_rcv_rate = -1.0;
    std::string accountnum = "";
};


class ServerManagerImpl : public monitor::proto::MonitorManager {
   public:
    ServerManagerImpl();
    virtual ~ServerManagerImpl();

    void SetMonitorInfo(::google::protobuf::RpcController* controller,
                        const ::monitor::proto::MonitorInfo* request,
                        ::google::protobuf::Empty* response,
                        ::google::protobuf::Closure* done);

    void GetMonitorInfo(::google::protobuf::RpcController* controller,
                        const ::google::protobuf::Empty* request,
                        ::monitor::proto::MonitorInfo* response,
                        ::google::protobuf::Closure* done);

    bool InsertOneInfo(monitor::proto::MonitorInfo& monitor_infos_);
    std::string SelectUserId(std::string accountNum);


    MidInfo parseInfos(monitor::proto::MonitorInfo& monitor_infos_);

   private:
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();
};
}  // namespace monitor