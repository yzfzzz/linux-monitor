#pragma once
#include <memory>
#include <mutex>
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
    std::string timeymd = "1970-1-1";
    std::string timehms = "00:00:00";
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
    bool isTableExist(std::string tableName,
                      std::shared_ptr<MysqlConn> conn_ptr);

    MidInfo parseInfos(monitor::proto::MonitorInfo& monitor_infos_);

   private:
    std::mutex m_create_mutex;
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();

    std::string create_subsql =
        std::string("(gpu_num int DEFAULT NULL,") +
        "gpu_name varchar(100) DEFAULT NULL," +
        "gpu_used_mem int DEFAULT NULL," + "gpu_total_mem int DEFAULT NULL," +
        "gpu_avg_util int DEFAULT NULL," +
        "cpu_load_avg_1 float DEFAULT NULL," +
        "cpu_load_avg_3 float DEFAULT NULL," +
        "cpu_load_avg_15 float DEFAULT NULL," + "mem_used float DEFAULT NULL," +
        "mem_total float DEFAULT NULL," + "net_send_rate float DEFAULT NULL," +
        "net_rcv_rate float DEFAULT NULL," + "user_id int NOT NULL," +
        "time time NOT NULL" +
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=" +
        "utf8mb4_0900_ai_ci COMMENT='create table according to date'";
};
}  // namespace monitor