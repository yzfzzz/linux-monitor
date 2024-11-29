#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include "gpu_monitor.h"
#include "json.hpp"
#include "monitor_info.pb.h"

monitor::GpuMonitor::GpuMonitor(const char* pipeName) {
    int ret = access(pipeName, F_OK);
    if (ret == -1) {
        printf("pipe isn't exit, create...\n");
        ret = mkfifo(pipeName, 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(-1);
        }
    }

    this->fd_ = open(pipeName, O_RDONLY);
    if (fd_ == -1) {
        std::cerr << "Error opening pipe." << std::endl;
        return;
    }
}

void monitor::GpuMonitor::updateOnce(
    monitor::proto::MonitorInfo* monitor_info) {
    char raw_buffer[1024];
    int read_bytes_size = 0;
    int max_attempts = 3;
    int attempt_count = 0;
    while (attempt_count < max_attempts) {
        read_bytes_size = read(this->fd_, raw_buffer, sizeof(raw_buffer) - 1);
        if (read_bytes_size > 0) {
            break;
        } else if (read_bytes_size == 0) {
            attempt_count += 1;

            std::this_thread::sleep_for(
                std::chrono::seconds(2 * attempt_count));
        }
    }
    if (attempt_count == max_attempts) {
        std::cout << "Error reading from pipe." << std::endl;

        return;
    }

    std::string str(raw_buffer);
    char buffer[1024];
    int first_index = str.find('[');
    int last_index = str.find(']', first_index);
    std::copy(str.begin() + first_index, str.begin() + last_index + 1, buffer);
    buffer[last_index - first_index + 1] = '\0';

    nlohmann::json data_list = nlohmann::json::parse(buffer);
    for (auto data : data_list) {
        gpu_id_ = data["ID"];
        gpu_name_ = data["DeviceName"];
        gpu_mem_total_ = data["MemoryTotal"];
        gpu_mem_free_ = data["MemoryFree"];
        gpu_mem_used_ = data["MemoryUsed"];
        gpu_mem_utilize_ = data["MemoryUtilization"];
        gpu_utilize_ = data["GPUUtilization"];
        temperture_ = data["Temperature"];
        fan_speed_ = data["FanSpeed"];
        power_stat_ = data["PowerStstus"];

        auto gpu_info_msg = monitor_info->add_gpu_info();
        gpu_info_msg->set_id(gpu_id_);
        gpu_info_msg->set_gpu_name(gpu_name_);
        gpu_info_msg->set_gpu_mem_total(gpu_mem_total_);
        gpu_info_msg->set_gpu_mem_free(gpu_mem_free_);
        gpu_info_msg->set_gpu_mem_used(gpu_mem_used_);
        gpu_info_msg->set_gpu_mem_utilize(gpu_mem_utilize_);
        gpu_info_msg->set_gpu_utilize(gpu_utilize_);
        gpu_info_msg->set_temperture(temperture_);
        gpu_info_msg->set_fan_speed(fan_speed_);
        gpu_info_msg->set_power_stat(power_stat_);
    }

    memset(buffer, 0, sizeof(buffer));
}