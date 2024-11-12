#include <iostream>
#include <mutex>
#include "json.hpp"
#include "rpc_manager.h"
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

void ServerManagerImpl::SetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response, ::google::protobuf::Closure* done) {
    mtx.lock();
    monitor_infos_.Clear();
    monitor_infos_ = *request;
    InsertOneInfo(monitor_infos_);
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

void ServerManagerImpl::GetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::google::protobuf::Empty* request,
    ::monitor::proto::MonitorInfo* response,
    ::google::protobuf::Closure* done) {
    mtx.lock();
    *response = monitor_infos_;
    done->Run();
    mtx.unlock();
}

ServerManagerImpl::MidInfo ServerManagerImpl::parseInfos(monitor::proto::MonitorInfo& monitor_infos_) {
    MidInfo mifo;
    minfo.gpu_num = monitor_info.gpu_info_size();
    minfo.gpu_name = monitor_info.gpu_info(0).gpu_name();
    minfo.gpu_used_mem = monitor_info.gpu_info(0).gpu_mem_used();
    minfo.gpu_total_mem = monitor_info.gpu_info(0).gpu_mem_total();
    minfo.gpu_avg_util = monitor_info.gpu_info(0).gpu_mem_utilize();
    minfo.cpu_load_avg_1 = monitor_info.cpu_load().load_avg_1();
    minfo.cpu_load_avg_3 = monitor_info.cpu_load().load_avg_3();
    minfo.cpu_load_avg_15 = monitor_info.cpu_load().load_avg_15();
    minfo.mem_used = monitor_info.mem_info().used_percent();
    minfo.mem_total = monitor_info.mem_info.total();
    minfo.net_send_rate = monitor_info.net_info(0).send_rate();
    minfo.net_rcv_rate = monitor_info.net_info(0).rcv_rate();

    minfo.ip_addr = monitor_info.ip_addr();
    return mifo;
}

bool ServerManagerImpl::InsertOneInfo(
    monitor::proto::MonitorInfo& monitor_infos_) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = "INSERT INTO table_20241111
                    (gpu_name, gpu_num, gpu_used_mem, gpu_avg_util, cpu_load_avg_1, cpu_load_avg_3, cpu_load_avg_15, mem_used, net_send_rate, net_rcv_rate, user_id)
                    VALUES(";
    MidInfo mifo = parseInfos(monitor_infos_);
    int user_id = SelectUserId(mifo.ip_addr);
    if (user_id == -1) {
        return false;
    }
    sql = sql+mifo.gpu_name+","+std::to_string(mifo.gpu_num)+","+std::to_string(mifo.gpu_used_mem)+","+
            std::to_string(mifo.gpu_avg_util)+","+std::to_string(mifo.cpu_load_avg_1)+","+std::to_string(mifo.cpu_load_avg_3)+","+
            std::to_string(mifo.cpu_load_avg_15)+","+std::to_string(mifo.mem_used)+","+std::to_string(mifo.net_send_rate)+","+
            std::to_string(mifo.net_rcv_rate)+","+std::to_string(user_id)+")";
    return conn_ptr->update(sql);
}

int ServerManagerImpl::SelectUserId(std::string ip) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = "SELECT id FROM `user` u WHERE ip_addr " + ip;
    int user_id;
    if (conn_ptr->update(sql) == true) {
        while (connptr->next()) {
            user_id = connptr->value(0);
        }
        return user_id;
    }
    return -1;
}

}  // namespace monitor