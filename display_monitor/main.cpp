#include "monitor_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monitor_widget w;
    w.show();
    return a.exec();
}
