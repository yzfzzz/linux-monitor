#include "cpu_load_monitor.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
#include "utils/read_file.h"

void monitor::CpuLoadMonitor::UpdateOnce(
    monitor::proto::MonitorInfo* monitor_info) {
    ReadFile cpu_load_file(string("/proc/loadavg"));
    vector<string> cpu_load;
    cpu_load_file.ReadLine(&cpu_load);

    // string => float
    m_load_avg_1 = stof(cpu_load[0]);
    m_load_avg_3 = stof(cpu_load[1]);
    m_load_avg_15 = stof(cpu_load[2]);

    auto cpu_load_msg = monitor_info->mutable_cpu_load();
    cpu_load_msg->set_load_avg_1(m_load_avg_1);
    cpu_load_msg->set_load_avg_3(m_load_avg_3);
    cpu_load_msg->set_load_avg_15(m_load_avg_15);

    return;
}