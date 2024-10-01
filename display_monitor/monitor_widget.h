#ifndef MONITOR_WIDGET_H
#define MONITOR_WIDGET_H

#include <QWidget>
#include <thread>
#include <string>
#include <QStandardItemModel>
#include <QStackedLayout>
#include "cpu_load_model.h"
#include "cpu_softirq_model.h"
#include "cpu_stat_model.h"
#include "mem_model.h"
#include "net_model.h"

using namespace std;
namespace monitor{

class MonitorWidget : public QWidget{

    Q_OBJECT

public:
    explicit MonitorWidget(QWidget* parent = nullptr);
    ~MonitorWidget();

    QWidget* ShowAllMonitorWidget(const string& name);
    QWidget* InitCpuMonitorWidget();
    QWidget* InitSoftIrqMonitorWidget();
    QWidget* InitMemMonitorWidget();
    QWidget* InitNetMonitorWidget();
    QWidget* InitButtonMenu(const string& name);

    void UpdateData(const monitor::proto::MonitorInfo& monitor_info);

private slots:
    void ClickCpuButton();
    void ClickSoftIrqButton();
    void ClickMemButton();
    void ClickNetButton();

private:
    QTableView* monitor_view_ = nullptr;
    QTableView* cpu_load_monitor_view_ = nullptr;
    QTableView* cpu_stat_monitor_view_ = nullptr;
    QTableView* mem_monitor_view_ = nullptr;
    QTableView* net_monitor_view_ = nullptr;

    MonitorBaseModel* monitor_model_ = nullptr;
    CpuLoadModel* cpu_load_model_ = nullptr;
    CpuStatModel* cpu_stat_model_ = nullptr;
    MemModel* mem_model_ = nullptr;
    NetModel* net_model_ = nullptr;

    QStackedLayout* stack_menu_ = nullptr;
};

}
#endif // MONITOR_WIDGET_H
