#pragma once
#include <string>
#include <unordered_map>

#include "monitor_info.pb.h"
#include "monitor_info.pb.h"
#include "monitor_inter.h"
using namespace std;
namespace monitor {

class CpuStatMonitor : public MonitorInter {
   public:
    struct CpuStat {
        string cpu_name;
        float user;
        float system;
        float idle;
        float nice;
        float io_wait;
        float irq;
        float soft_irq;
        float steal;
        float guest;
        float guest_nice;
    };

    CpuStatMonitor() {}
    void updateOnce(monitor::proto::MonitorInfo* monitor_info);
    void stop() override {}

   private:
    unordered_map<string, struct CpuStat> cpu_stat_map_;
};

};  // namespace monitor