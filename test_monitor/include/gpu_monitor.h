#pragma once
#include <string>
#include <unordered_map>
#include "monitor_inter.h"
#include "log.h"
// TODO: 将py文件的mem部分值优化一下
namespace monitor {
class GpuMonitor : public MonitorInter {
   public:
    GpuMonitor(char* pipeName);
    void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
    void Stop() override {}

   private:
    std::string gpu_id;
    std::string gpu_name;
    int gpu_mem_total;
    int gpu_mem_free;
    int gpu_mem_used;
    int gpu_mem_utilize;
    int gpu_utilize;
    int temperture;
    int fan_speed;
    int power_stat;
    int fd;
};
}  // namespace monitor