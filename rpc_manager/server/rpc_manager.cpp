#include <iostream>
#include "rpc_manager.h"
#include <mutex>
using namespace std;
mutex mtx;
namespace monitor {

ServerManagerImpl::ServerManagerImpl() {}
ServerManagerImpl::~ServerManagerImpl() {}

// ::grpc::Status ServerManagerImpl::SetMonitorInfo(
//     ::grpc::ServerContext* context,
//     const ::monitor::proto::MonitorInfo* request,
//     ::google::protobuf::Empty* response) {
//     monitor_infos_.Clear();
//     monitor_infos_ = *request;

//     cout << "jinru" << request->soft_irq_size() << endl;
//     return grpc::Status::OK;
// }

void ServerManagerImpl::SetMonitorInfo(::google::protobuf::RpcController* controller,
                                       const ::monitor::proto::MonitorInfo* request,
                                       ::google::protobuf::Empty* response,
                                       ::google::protobuf::Closure* done) {
    mtx.lock();
    monitor_infos_.Clear();
    monitor_infos_ = *request;

    cout << "jinru" << request->soft_irq_size() << endl;
    done->Run();
    mtx.unlock();
}

// ::grpc::Status ServerManagerImpl::GetMonitorInfo(
//     ::grpc::ServerContext* context, const ::google::protobuf::Empty* request,
//     ::monitor::proto::MonitorInfo* response) {
//     *response = monitor_infos_;
//     return grpc::Status::OK;
// }

void ServerManagerImpl::GetMonitorInfo(::google::protobuf::RpcController* controller,
                                       const ::google::protobuf::Empty* request,
                                       ::monitor::proto::MonitorInfo* response,
                                       ::google::protobuf::Closure* done) {
    mtx.lock();
    *response = monitor_infos_;
    done->Run();
    mtx.unlock();
}

}  // namespace monitor