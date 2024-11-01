#include "mprpclog.h"

// TODO: 将日志模块加入到框架里
void SetupLogging(const std::string logDir) {
    // 设置日志输出目录
    FLAGS_log_dir = logDir;

    // 设置日志级别
    FLAGS_stderrthreshold = google::INFO;

    // 设置日志文件大小为10MB
    FLAGS_max_log_size = 10 * 1024;  // 10MB

    // 禁用日志文件名中的机器名后缀
    FLAGS_alsologtostderr = true;
    FLAGS_logtostderr = false;

    FLAGS_colorlogtostderr = true;
    // 日志最多缓存的秒数，0表示实时输出
    FLAGS_logbufsecs = 30;
}

void LogInit(char** argv, std::string logPath) {
    // 设置日志目录
    std::string logDir = logPath;
    // 初始化日志
    SetupLogging(logDir);
    // 初始化 Glog
    google::InitGoogleLogging(argv[0]);
}