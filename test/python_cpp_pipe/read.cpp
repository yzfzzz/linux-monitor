#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include "json.hpp"

using json = nlohmann::json;

int main()
{
    const char *pipeName = "py_cpp_pipe.fifo";

    // 判断读管道是否存在，不存在则创建
    int ret = access(pipeName, F_OK);
    if (ret == -1)
    {
        printf("pipe isn't exit, create...\n");
        ret = mkfifo(pipeName, 0664);
        if (ret == -1)
        {
            perror("mkfifo");
            exit(-1);
        }
    }
    // 打开有名管道进行读取
    int fd = open(pipeName, O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Error opening pipe." << std::endl;
        return 1;
    }

    while (1)
    {
        char buffer[1024];
        int bytesRead = 0;
        int max_attempts = 3;
        int attempt_count = 0;
        while (attempt_count < max_attempts)
        {
            bytesRead = read(fd, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0)
            {
                break;
            }
            else if (bytesRead == 0)
            {
                std::cout << "Accidentally disconnected!" << std::endl;
                attempt_count += 1;
                std::this_thread::sleep_for(std::chrono::seconds(2 * attempt_count));
            }
        }
        if (attempt_count == max_attempts)
        {
            std::cout << "Error reading from pipe." << std::endl;
            return 1;
        }

        buffer[bytesRead] = '\0';
        // 解析 JSON 数据
        json data = json::parse(buffer);

        std::string DeviceName = data["DeviceName"];
        long long MemoryTotal = data["MemoryTotal"];
        long long MemoryFree = data["MemoryFree"];
        long long MemoryUsed = data["MemoryUsed"];
        int MemoryUtilization = data["MemoryUtilization"];
        int GPUUtilization = data["GPUUtilization"];
        int Temperature = data["Temperature"];
        int FanSpeed = data["FanSpeed"];
        int PowerStstus = data["PowerStstus"];

        std::cout << "Received DeviceName: " << DeviceName << std::endl
                  << "MemoryTotal" << MemoryTotal << std::endl
                  << "MemoryFree" << MemoryFree << std::endl
                  << "GPUUtilization" << GPUUtilization << std::endl
                  << "Temperature" << Temperature << std::endl
                  << "FanSpeed" << FanSpeed << std::endl;

        std::cout << "--------------------" << std::endl;

        memset(buffer, 0, sizeof(buffer));
        // 睡眠 3 秒钟
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "After sleeping for 3 seconds." << std::endl;
    }

    close(fd);

    return 0;
}