#include "start.h"
#include "ui_start.h"
#include <iostream>

Start::Start(int argc, char** argv, QWidget* parent)
    : QWidget(parent), ui(new Ui::start) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->login_w, &Login::loginJump, this,
            [=](std::string account_num,
                std::vector<std::string> machine_name_array) {
                for (int i = 0; i < machine_name_array.size(); i++) {
                    std::cout << "Run Start func: "<<machine_name_array[i] << std::endl;
                    monitor_map[machine_name_array[i]] = new Widget(
                        argc, argv, account_num, machine_name_array[i]);
                    ui->stackedWidget->addWidget(
                        monitor_map[machine_name_array[i]]);
                }
                ui->stackedWidget->setCurrentWidget(
                    monitor_map[machine_name_array[0]]);
            });
    // ui->stackedWidget->setCurrentIndex(2);
}

Start::~Start() { delete ui; }
