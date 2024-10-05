#include "cpu_stat_monitor.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
#include "utils/read_file.h"
/*
cpu  2479210 1053 3027543 406505867 1316138 0 20917 0 0 0
cpu0 1238518 629 1512236 203070925 828212 0 11123 0 0 0
cpu1 1240692 424 1515307 203434942 487926 0 9793 0 0 0
intr 1633596168 0 9 0 0 90 0 3 0 0 0 0 0 15 0 2056560 0 0 0 0 0 0 0 0 0 0
4282828 1 4878293 1 0 22673825 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ctxt 3200256211 btime 1724829399
processes 14472391
procs_running 1
procs_blocked 0
softirq 411065353 1 148652601 0 10741064 1028511 0 16 150044884 0 100598276
*/

// CPU性能
void monitor::CpuStatMonitor::UpdateOnce(
    monitor::proto::MonitorInfo* monitor_info) {
    ReadFile cpu_stat_file(string("/proc/stat"));
    vector<string> cpu_stat_list;
    while (cpu_stat_file.ReadLine(&cpu_stat_list)) {
        if (cpu_stat_list[0].find("cpu") != string::npos) {
            struct CpuStat cpu_stat;
            cpu_stat.cpu_name = cpu_stat_list[0];
            cpu_stat.user = stof(cpu_stat_list[1]);
            cpu_stat.nice = stof(cpu_stat_list[2]);
            cpu_stat.system = stof(cpu_stat_list[3]);
            cpu_stat.idle = stof(cpu_stat_list[4]);
            cpu_stat.io_wait = stof(cpu_stat_list[5]);
            cpu_stat.irq = stof(cpu_stat_list[6]);
            cpu_stat.soft_irq = stof(cpu_stat_list[7]);
            cpu_stat.steal = stof(cpu_stat_list[8]);
            cpu_stat.guest = stof(cpu_stat_list[9]);
            cpu_stat.guest_nice = stof(cpu_stat_list[10]);

            auto it = m_cpu_stat_map.find(cpu_stat.cpu_name);
            if (it != m_cpu_stat_map.end()) {
                struct CpuStat old = it->second;
                auto cpu_stat_msg = monitor_info->add_cpu_stat();
                // ??? 性能计算
                float new_cpu_total_time = cpu_stat.user + cpu_stat.system +
                                           cpu_stat.idle + cpu_stat.nice +
                                           cpu_stat.io_wait + cpu_stat.irq +
                                           cpu_stat.soft_irq + cpu_stat.steal;
                float old_cpu_total_time = old.user + old.system + old.idle +
                                           old.nice + old.io_wait + old.irq +
                                           old.soft_irq + old.steal;
                float new_cpu_busy_time = cpu_stat.user + cpu_stat.system +
                                          cpu_stat.nice + cpu_stat.irq +
                                          cpu_stat.soft_irq + cpu_stat.steal;
                float old_cpu_busy_time = old.user + old.system + old.nice +
                                          old.irq + old.soft_irq + old.steal;

                float cpu_percent = (new_cpu_busy_time - old_cpu_busy_time) /
                                    (new_cpu_total_time - old_cpu_total_time) *
                                    100.00;
                float cpu_user_percent =
                    (cpu_stat.user - old.user) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;
                float cpu_system_percent =
                    (cpu_stat.system - old.system) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;
                float cpu_nice_percent =
                    (cpu_stat.nice - old.nice) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;
                float cpu_idle_percent =
                    (cpu_stat.idle - old.idle) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;
                float cpu_io_wait_percent =
                    (cpu_stat.io_wait - old.io_wait) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;
                float cpu_irq_percent =
                    (cpu_stat.irq - old.irq) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;
                float cpu_soft_irq_percent =
                    (cpu_stat.soft_irq - old.soft_irq) /
                    (new_cpu_total_time - old_cpu_total_time) * 100.00;

                // 保存为proto格式
                cpu_stat_msg->set_cpu_name(cpu_stat.cpu_name);
                cpu_stat_msg->set_cpu_percent(cpu_percent);
                cpu_stat_msg->set_usr_percent(cpu_user_percent);
                cpu_stat_msg->set_system_percent(cpu_system_percent);
                cpu_stat_msg->set_nice_percent(cpu_nice_percent);
                cpu_stat_msg->set_idle_percent(cpu_idle_percent);
                cpu_stat_msg->set_io_wait_percent(cpu_io_wait_percent);
                cpu_stat_msg->set_irq_percent(cpu_irq_percent);
                cpu_stat_msg->set_soft_irq_percent(cpu_soft_irq_percent);
            }
            m_cpu_stat_map[cpu_stat.cpu_name] = cpu_stat;
        }
        cpu_stat_list.clear();
    }
}