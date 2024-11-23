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
            for (int i = 0; i < 30; i++) {
                w.Update(queryData.queryData_array[i]);

                // TODO: 为什么加上这一句就可以了？
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        while (true) {
            int n = 1;
            queryData.queryData_array.clear();
            if (queryData.queryDataInfo("12345678", n)) {
                for (int i = 0; i < n; i++) {
                    w.Update(queryData.queryData_array[i]);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    thread_->detach();

    return a.exec();
}
