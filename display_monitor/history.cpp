#include "get_time.h"
#include "history.h"
#include "log.h"
namespace monitor {
std::vector<MidInfo> History::getHistoryInfo(std::string account_num,
                                             int count) {
    // sql = select * from table_20241119 WHERE user_id = 1 order by time desc
    // limit 10
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    get_curTime cur_time;
    MidInfo midinfo;
    std::vector<MidInfo> history_array;
    std::string table_name = "table_" + cur_time.get_year_mon_day();
    std::string select_count = std::to_string(count);
    std::string user_id = SelectUserId(account_num);
    std::string sql = "select * from " + table_name +
                      " WHERE user_id = " + user_id +
                      " order by time desc limit " + select_count;

    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            midinfo.gpu_num = std::stoi(conn_ptr->value(0));
            midinfo.gpu_name = conn_ptr->value(1);
            midinfo.gpu_used_mem = std::stoi(conn_ptr->value(2));
            midinfo.gpu_total_mem = std::stoi(conn_ptr->value(3));
            midinfo.gpu_avg_util = std::stoi(conn_ptr->value(4));
            midinfo.cpu_load_avg_1 = std::stof(conn_ptr->value(5));
            midinfo.cpu_load_avg_3 = std::stof(conn_ptr->value(6));
            midinfo.cpu_load_avg_15 = std::stof(conn_ptr->value(7));
            midinfo.mem_used = std::stof(conn_ptr->value(8));
            midinfo.mem_total = std::stof(conn_ptr->value(9));
            midinfo.net_send_rate = std::stof(conn_ptr->value(10));
            midinfo.net_rcv_rate = std::stof(conn_ptr->value(11));
            midinfo.timeymd = cur_time.get_year_mon_day();
            midinfo.timehms = conn_ptr->value(13);

            history_array.push_back(midinfo);
        }
    }
    return history_array;
}

std::string History::SelectUserId(std::string accountNum) {
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