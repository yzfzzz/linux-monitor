#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "gpu_monitor.h"
#include "json.hpp"
#include "monitor_info.pb.h"

using json = nlohmann::json;

monitor::GpuMonitor::GpuMonitor(char* pipeName) {
    // 判断读管道是否存在，不存在则创建
    int ret = access(pipeName, F_OK);
    if (ret == -1) {
        printf("pipe isn't exit, create...\n");
        ret = mkfifo(pipeName, 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(-1);
        }
    }
    // 打开有名管道进行读取
    this->fd = open(pipeName, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening pipe." << std::endl;
        return;
    }
}

void monitor::GpuMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
    char buffer[1024];
    int bytesRead = 0;
    int max_attempts = 3;
    int attempt_count = 0;
    while (attempt_count < max_attempts) {
        bytesRead = read(this->fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            break;
        } else if (bytesRead == 0) {
            std::cout << "Accidentally disconnected!" << std::endl;
            attempt_count += 1;
            std::this_thread::sleep_for(std::chrono::seconds(2 * attempt_count));
        }
    }
    if (attempt_count == max_attempts) {
        std::cout << "Error reading from pipe." << std::endl;
        return;
    }

    buffer[bytesRead] = '\0';
    // 解析 JSON 数据
    json data_list = json::parse(buffer);
    for (auto data : data_list) {
        gpu_id = data["ID"];
        gpu_name = data["DeviceName"];
        gpu_mem_total = data["MemoryTotal"];
        gpu_mem_free = data["MemoryFree"];
        gpu_mem_used = data["MemoryUsed"];
        gpu_mem_utilize = data["MemoryUtilization"];
        gpu_utilize = data["GPUUtilization"];
        temperture = data["Temperature"];
        // 这里我的显卡没有风扇
        fan_speed = 0;
        power_stat = data["PowerStstus"];

        // TODO:查一下是add还是mutable
        auto gpu_info_msg = monitor_info->add_gpu_info();
        gpu_info_msg->set_id(gpu_id);
        gpu_info_msg->set_gpu_name(gpu_name);
        gpu_info_msg->set_gpu_mem_total(gpu_mem_total);
        gpu_info_msg->set_gpu_mem_free(gpu_mem_free);
        gpu_info_msg->set_gpu_mem_used(gpu_mem_used);
        gpu_info_msg->set_gpu_mem_utilize(gpu_mem_utilize);
        gpu_info_msg->set_gpu_utilize(gpu_utilize);
        gpu_info_msg->set_temperture(temperture);
        gpu_info_msg->set_fan_speed(fan_speed);
        gpu_info_msg->set_power_stat(power_stat);
    }

    memset(buffer, 0, sizeof(buffer));
}