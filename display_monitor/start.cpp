#include "start.h"
#include "ui_start.h"

Start::Start(int argc, char** argv, QWidget* parent)
    : QWidget(parent), ui(new Ui::start) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->login_w, &Login::loginJump, this, [=](std::string account_num) {
        monitor_w = new Widget(argc, argv, account_num);
        ui->stackedWidget->addWidget(monitor_w);
        ui->stackedWidget->setCurrentIndex(2);
    });
    // ui->stackedWidget->setCurrentIndex(2);
}

Start::~Start() { delete ui; }
