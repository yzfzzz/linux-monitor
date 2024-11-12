#include <iostream>
#include <mutex>
#include <iostream>
#include "json.hpp"
#include "rpc_manager.h"
#include "log.h"
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
    LOG(INFO) << "RPC Call: ServerManagerImpl::SetMonitorInfo";
    monitor_infos_.Clear();
    monitor_infos_ = *request;
    InsertOneInfo(monitor_infos_);
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
    LOG(INFO) << "RPC Call: ServerManagerImpl::GetMonitorInfo";
    *response = monitor_infos_;
    done->Run();
    mtx.unlock();
}

MidInfo ServerManagerImpl::parseInfos(monitor::proto::MonitorInfo& monitor_infos_) {
    MidInfo mifo;
    mifo.gpu_num = monitor_infos_.gpu_info_size();
    if(monitor_infos_.gpu_info_size() > 0)
    {
        mifo.gpu_name = monitor_infos_.gpu_info(0).gpu_name();
        mifo.gpu_used_mem = monitor_infos_.gpu_info(0).gpu_mem_used();
        mifo.gpu_total_mem = monitor_infos_.gpu_info(0).gpu_mem_total();
        mifo.gpu_avg_util = monitor_infos_.gpu_info(0).gpu_mem_utilize();
    }
    mifo.cpu_load_avg_1 = monitor_infos_.cpu_load().load_avg_1();
    mifo.cpu_load_avg_3 = monitor_infos_.cpu_load().load_avg_3();
    mifo.cpu_load_avg_15 = monitor_infos_.cpu_load().load_avg_15();
    mifo.mem_used = monitor_infos_.mem_info().used_percent();
    mifo.mem_total = monitor_infos_.mem_info().total();
    if(monitor_infos_.net_info_size() > 0)
    {
        mifo.net_send_rate = monitor_infos_.net_info(0).send_rate();
        mifo.net_rcv_rate = monitor_infos_.net_info(0).rcv_rate();
            }
    mifo.accountnum = monitor_infos_.accountnum();
    LOG(INFO) << "Parse monitor_infos_...";
    return mifo;
}

bool ServerManagerImpl::InsertOneInfo(
    monitor::proto::MonitorInfo& monitor_infos_) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = "INSERT INTO "+std::string("table_20241111")+"(gpu_name, gpu_num, gpu_used_mem, gpu_total_mem, gpu_avg_util, cpu_load_avg_1, cpu_load_avg_3, cpu_load_avg_15, mem_used,mem_total , net_send_rate, net_rcv_rate, user_id) "+"VALUES(";
    MidInfo mifo = parseInfos(monitor_infos_);
    std::string user_id = SelectUserId(mifo.accountnum);
    if (user_id.empty()==true) {
        return false;
    }
    sql = sql+"'"+mifo.gpu_name+"'"+","+std::to_string(mifo.gpu_num)+","+std::to_string(mifo.gpu_used_mem)+","+std::to_string(mifo.gpu_total_mem)+","+
            std::to_string(mifo.gpu_avg_util)+","+std::to_string(mifo.cpu_load_avg_1)+","+std::to_string(mifo.cpu_load_avg_3)+","+
            std::to_string(mifo.cpu_load_avg_15)+","+std::to_string(mifo.mem_used)+","+std::to_string(mifo.mem_total)+","+std::to_string(mifo.net_send_rate)+","+
            std::to_string(mifo.net_rcv_rate)+","+user_id+")";
    OG(INFO) << sql;
    if(conn_ptr->update(sql))
    {
        LOG(INFO) << "Succeed to insert one sql";
        return true;
    }
    LOG(WARNING) << "Failed to insert one sql";
    return false;
}

std::string ServerManagerImpl::SelectUserId(std::string accountNum) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = "SELECT id FROM `user` u WHERE accountnum =" + accountNum;
    std::string user_id;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            user_id = conn_ptr->value(0);
        }
        LOG(INFO) << "Succeed to select user_id";
        return user_id;
    }
    LOG(WARNING) << "Failed to request user_id!";
    return "";
}

}  // namespace monitor