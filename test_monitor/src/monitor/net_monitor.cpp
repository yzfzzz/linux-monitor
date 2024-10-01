#include "net_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"

void monitor::NetMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
{
    ReadFile net_file(std::string("/proc/net/dev"));
    vector<string> net_datas;
    while (net_file.ReadLine(&net_datas))
    {
        string name = net_datas[0];
        // 是eth0:、docker0:、lo:这三个就进入while循环中
        if (name.find(':') == name.size() - 1 && net_datas.size() >= 10)
        {
            struct NetInfo net_info;
            name.pop_back();
            net_info.name = name;
            net_info.rcv_bytes = stoll(net_datas[1]);
            net_info.rcv_packets = stoll(net_datas[2]);
            net_info.err_in = stoll(net_datas[3]);
            net_info.drop_in = stoll(net_datas[4]);
            net_info.snd_bytes = stoll(net_datas[9]);
            net_info.snd_packets = stoll(net_datas[10]);
            net_info.err_out = stoll(net_datas[11]);
            net_info.drop_out = stoll(net_datas[12]);
            net_info.timepoint = boost::chrono::steady_clock::now();

            auto iter = m_net_info.find(name);
            if (iter != m_net_info.end())
            {
                struct NetInfo old = move(iter->second);
                double period =
                    Utils::SteadyTimeSecond(net_info.timepoint, old.timepoint);
                auto one_net_msg = monitor_info->add_net_info();
                one_net_msg->set_name(net_info.name);
                one_net_msg->set_send_rate((net_info.snd_bytes - old.snd_bytes) /
                                           1024.0 / period);
                one_net_msg->set_rcv_rate((net_info.rcv_bytes - old.rcv_bytes) /
                                          1024.0 / period);
                one_net_msg->set_send_packets_rate(
                    (net_info.snd_packets - old.snd_packets) / period);
                one_net_msg->set_rcv_packets_rate(
                    (net_info.rcv_packets - old.rcv_packets) / period);
            }
            m_net_info[name] = net_info;
        }
        net_datas.clear();
    }
}
