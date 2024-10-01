#pragma once

#include <string>
#include "monitor_inter.h"

namespace monitor{

// 多态
class CpuLoadMonitor: public MonitorInter{
public:
    CpuLoadMonitor(){}
    // 重写虚函数
    void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
    // override：这是 C++11 引入的一个关键字，用于明确指出这个成员函数是基类中某个虚函数的重写
    void Stop() override {}


private:
    float m_load_avg_1;
    float m_load_avg_3;
    float m_load_avg_15;
};

};