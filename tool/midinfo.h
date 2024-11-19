#pragma once
#include <string>
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
}  // namespace monitor