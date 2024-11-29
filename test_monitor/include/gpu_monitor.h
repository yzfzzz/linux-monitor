#pragma once
#include <string>
#include <unordered_map>
#include "log.h"
#include "monitor_inter.h"
// TODO: 将py文件的mem部分值优化一下
namespace monitor {
class GpuMonitor : public MonitorInter {
   public:
    GpuMonitor(const char* pipeName);
    void updateOnce(monitor::proto::MonitorInfo* monitor_info);
    void stop() override {}

   private:
    std::string gpu_id_;
    std::string gpu_name_;
    int gpu_mem_total_;
    int gpu_mem_free_;
    int gpu_mem_used_;
    int gpu_mem_utilize_;
    int gpu_utilize_;
    int temperture_;
    int fan_speed_;
    int power_stat_;
    int fd_;
};
}  // namespace monitor