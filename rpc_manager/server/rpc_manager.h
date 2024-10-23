#pragma once
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <unordered_map>
#include "monitor_info.pb.h"

namespace monitor {
class ServerManagerImpl : public monitor::proto::GrpcManager {
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

   private:
    monitor::proto::MonitorInfo monitor_infos_;
};
}  // namespace monitor