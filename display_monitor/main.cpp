#include <QApplication>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "query_data.h"
#include "widget.h"
#include <mutex>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Widget w;
    w.show();
    std::unique_ptr<std::thread> thread_;
    bool flag = false;
    thread_ = std::make_unique<std::thread>([&]() {
        while (true) {
            int n = 1;
            if(!flag)
            {
                n = 30;
                flag = true;
            } 
            monitor::queryData queryData;
            if (queryData.queryDataInfo("12345678", n)) {
                for (int i = 0; i < n; i++) {
                    w.Update(queryData.queryData_array[i]);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });
    thread_->detach();

    return a.exec();
}
