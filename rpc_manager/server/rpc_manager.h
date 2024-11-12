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

    void InsertOneInfo();
    int SelectUserId();

    struct MidInfo {
        int gpu_num;
        string gpu_name;
        int gpu_used_mem;
        int gpu_total_mem;
        int gpu_avg_util;
        float cpu_load_avg_1;
        float cpu_load_avg_3;
        float cpu_load_avg_15;
        float mem_used;
        float mem_total;
        float net_send_rate;
        float net_rcv_rate;

        string ip_addr
    };

    MidInfo parseInfos(monitor::proto::MonitorInfo request);

   private:
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();
};
}  // namespace monitor