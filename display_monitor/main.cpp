
#include <QApplication>
#include <thread>
#include "mprpcapplication.h"
#include "client/rpc_client.h"
#include "monitor_widget.h"
#include "history.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MprpcApplication::Init(argc, argv);

    std::string server_address = "localhost:50051";

    monitor::MonitorWidget moitor_widget;
    monitor::RpcClient rpc_client(server_address);
    monitor::proto::MonitorInfo monitor_info;

    // get board name
    rpc_client.GetMonitorInfo(&monitor_info);
    std::string name = monitor_info.name();

    QWidget* widget = moitor_widget.ShowAllMonitorWidget(name);
    widget->show();

    std::unique_ptr<std::thread> thread_;
    monitor::History history;
    thread_ = std::make_unique<std::thread>([&]() {
        while (true) {

            std::vector<monitor::MidInfo> history_infos = history.getHistoryInfo("12345678",10);
            for(int i = 0; i < 10;i++)
            {
                std::cout << history_infos[i].gpu_name << " | " << history_infos[i].cpu_load_avg_15<< " | " << history_infos[i].timehms << std::endl;
            }

            // monitor_info.Clear();
            // rpc_client.GetMonitorInfo(&monitor_info);

            // moitor_widget.UpdateData(monitor_info);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });
    thread_->detach();

    return app.exec();
}