#pragma once
#include <string>
#include "monitor_inter.h"

namespace monitor {

class CpuLoadMonitor : public MonitorInter {
   public:
    CpuLoadMonitor() {}
    // 重写虚函数
    void updateOnce(monitor::proto::MonitorInfo* monitor_info);
    // override：这是 C++11
    // 引入的一个关键字，用于明确指出这个成员函数是基类中某个虚函数的重写
    void stop() override {}

   private:
    float load_avg_1_;
    float load_avg_3_;
    float load_avg_15_;
};

};  // namespace monitor