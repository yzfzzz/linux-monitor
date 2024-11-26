#include <QApplication>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "query_data.h"
#include "widget.h"
#include "client/rpc_client.h"
#include "mprpcapplication.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MprpcApplication::Init(argc, argv);
    std::string server_address = "localhost:50051";
    monitor::RpcClient rpc_client(server_address);
    Widget w;
    w.show();
    std::unique_ptr<std::thread> thread_;
    thread_ = std::make_unique<std::thread>([&]() {
        monitor::queryData queryData;
        if (queryData.queryDataInfo("12345678", 30, rpc_client)) {
            w.Update(queryData.queryData_array);
        }
        while (true) {
            int n = 1;
            queryData.queryData_array.clear();
            if (queryData.queryDataInfo("12345678", n, rpc_client)) {
                w.Update(queryData.queryData_array);
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });
    thread_->detach();

    return a.exec();
}
