#include "cpu_load_monitor.h"
#include "monitor_info.pb.h"
#include "utils/read_file.h"


void monitor::CpuLoadMonitor::updateOnce(
    monitor::proto::MonitorInfo* monitor_info) {
    ReadFile cpu_load_file(string("/proc/loadavg"));
    vector<string> cpu_load;
    cpu_load_file.readLine(&cpu_load);

    // string => float
    load_avg_1_ = stof(cpu_load[0]);
    load_avg_3_ = stof(cpu_load[1]);
    load_avg_15_ = stof(cpu_load[2]);

    auto cpu_load_msg = monitor_info->mutable_cpu_load();
    cpu_load_msg->set_load_avg_1(load_avg_1_);
    cpu_load_msg->set_load_avg_3(load_avg_3_);
    cpu_load_msg->set_load_avg_15(load_avg_15_);

    return;
}