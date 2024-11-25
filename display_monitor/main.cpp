#include <QApplication>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "query_data.h"
#include "widget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Widget w;
    w.show();
    std::unique_ptr<std::thread> thread_;
    thread_ = std::make_unique<std::thread>([&]() {
        monitor::queryData queryData;
        if (queryData.queryDataInfo("12345678", 30)) {
            w.Update(queryData.queryData_array);
        }
        while (true) {
            int n = 1;
            queryData.queryData_array.clear();
            if (queryData.queryDataInfo("12345678", n)) {
                w.Update(queryData.queryData_array);
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });
    thread_->detach();

    return a.exec();
}
