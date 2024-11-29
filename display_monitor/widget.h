#ifndef WIDGET_H
#define WIDGET_H

#include <QString>
#include <QWidget>
#include <mutex>
#include "midinfo.h"
#include "water_progress.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

   public:
    Widget(int argc, char **argv, QWidget *parent = nullptr);
    ~Widget();
    void update(std::vector<monitor::MidInfo> &midinfo_array);

   public:
    void recvLabelDownStr(QVector<QString> qstr_array);
    void recvLabelDownQss(QVector<QString> qss_array);
    void recvWaterValue(QVector<int> water_array);
    void recvGpuInfos(QVector<QString> gpu_infos_array);
    void recvNetList(QList<QPointF> send_list, QList<QPointF> recv_list);
    void recvGpuList(QList<QPointF> gpu_list);
   signals:
    void set_workthread_arg(int argc, char **argv);

   private:
    Ui::Widget *ui;
};
#endif  // WIDGET_H
