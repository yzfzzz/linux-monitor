#ifndef START_H
#define START_H

#include <QWidget>
#include <widget.h>

namespace Ui {
class start;
}

class Start : public QWidget {
    Q_OBJECT

   public:
    explicit Start(int argc,char** argv,QWidget *parent = nullptr);
    ~Start();
    Widget* monitor_w;

   private:
    Ui::start *ui;
};

#endif  // START_H
