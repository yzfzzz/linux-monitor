#pragma once
#include <string>
#include <unordered_map>

#include "monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
using namespace std;
namespace monitor{

class CpuStatMonitor: public MonitorInter{
public:
    struct CpuStat{
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
    void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
    void Stop() override {}

private:
    unordered_map<string, struct CpuStat> m_cpu_stat_map;
};

};