#ifndef START_H
#define START_H

#include <QWidget>
#include <widget.h>
#include <unordered_map>
#include <string>

namespace Ui {
class start;
}

class Start : public QWidget {
    Q_OBJECT

   public:
    explicit Start(int argc,char** argv,QWidget *parent = nullptr);
    ~Start();
    std::unordered_map<std::string, Widget*> monitor_map;

   private:
    Ui::start *ui;
};

#endif  // START_H
