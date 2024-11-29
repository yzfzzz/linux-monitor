#include <unistd.h>
#include <iostream>
#include <string>
#include "mprpcapplication.h"

MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp() {
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char** argv) {
    if (argc < 2) {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    char c = 0;
    std::string config_file;
    // 输入: ./a.out -i xxxxxx
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':':
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
    // 开始加载配置文件了
    /* config
    rpcserver_ip
    rpcserver_port
    zookeeper_ip
    zookeeper_port
    */
    m_config.LoadConfigFile(config_file.c_str());
    std::cout << "rpcserverip = " << m_config.Load("rpcserverip") << std::endl;
    std::cout << "rpcserverport = " << m_config.Load("rpcserverport")
              << std::endl;
    std::cout << "zookeeperip = " << m_config.Load("zookeeperip") << std::endl;
    std::cout << "zookeeperport = " << m_config.Load("zookeeperport")
              << std::endl;
}

MprpcConfig& MprpcApplication::GetConfig() { return m_config; }
