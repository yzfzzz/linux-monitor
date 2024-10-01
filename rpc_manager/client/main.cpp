#pragma once
#include "monitor_info.pb.h"
#include <string>
#include "rpc_client.h"
#include "monitor_info.grpc.pb.h"

int main(int argc, char* argv[])
{
    monitor::proto::MonitorInfo monitor_info;
    auto soft_irq = monitor_info.add_soft_irq();
    auto soft_irq->set_cpu("cpu1");

    auto soft_irq2 = monitor_info.add_soft_irq();
    soft_irq2->set_cpu("cpu2");

    monitor::RpcClient rpc_client;
    rpc_client.SetMonitorInfo(monitor_info);

    return 0;
}