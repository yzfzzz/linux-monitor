#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <vector>
#include "client/rpc_client.h"
#include "mprpcapplication.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QWidget {
    Q_OBJECT

   public:
    Login(QWidget* parent = nullptr);
    ~Login();

    monitor::RpcClient* rpc_client_ptr;

   signals:
   void loginJump(std::string account_num, std::vector<std::string> machine_name_array);

   private slots:
    void on_btn_login_clicked();

   private:
    Ui::Login* ui;
};
#endif  // LOGIN_H
