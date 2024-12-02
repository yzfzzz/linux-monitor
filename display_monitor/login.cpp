#include <QDebug>
#include <QString>
#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) : QWidget(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    std::string server_address = "localhost:50051";
    rpc_client_ptr = new monitor::RpcClient(server_address);
}

Login::~Login() { delete ui; }

void Login::on_btn_login_clicked() {
    qDebug() << "登录按钮被点击";
    std::string user_name = ui->lineE_user_name->text().toStdString();
    std::string pwd = ui->lineE_pwd->text().toStdString();
    bool isChecked = ui->btn_register->isChecked();
    if (isChecked) {

        qDebug() << "复选框已被勾选";
        // 注册
        if (user_name.empty() && (!pwd.empty())) {
            ::monitor::proto::UserMessage request;
            ::monitor::proto::UserResponseMessage response;
            request.set_account_num(user_name);
            request.set_pwd(pwd);
            rpc_client_ptr->LoginRegister(request, response);
            std::string response_str = response.response_str();
            std::cout << response_str  << std::endl;
            QString notice = QString::fromStdString(response_str);
            ui->label_notice_text->setText(notice);
            // 跳转
        } else if (user_name.empty() && pwd.empty()) {
            QString notice = "密码为空，无法注册";
            ui->label_notice_text->setText(notice);
        } else if (!user_name.empty()) {
            QString notice = "账号不为空，无法注册";
            ui->label_notice_text->setText(notice);
        }
    } else {

        qDebug() << "复选框未被勾选";
        if (user_name.empty()) {
            QString notice = "请输入你的账号";
            ui->label_notice_text->setText(notice);
        } else if (pwd.empty()) {
            QString notice = "请输入你的密码";
            ui->label_notice_text->setText(notice);
        } else {
            // 登录功能
            monitor::proto::UserMessage request;
            monitor::proto::UserResponseMessage response;
            request.set_account_num(user_name);
            request.set_pwd(pwd);
            rpc_client_ptr->LoginRegister(request, response);
            std::string response_str = response.response_str();
            std::cout << response_str  << std::endl;
            QString notice;
            if (response_str == "login successful") {
                notice = QString::fromStdString(response_str);
                emit loginJump(user_name);
            } else {
                notice = QString::fromStdString(response_str);
            }
            ui->label_notice_text->setText(notice);
        }
    }
}
