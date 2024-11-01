#ifndef MONITOR_WIDGET_H
#define MONITOR_WIDGET_H

#include <QStackedLayout>
#include <QStandardItemModel>
#include <QtWidgets>
#include <string>
#include <thread>
#include "cpu_load_model.h"
#include "cpu_softirq_model.h"
#include "cpu_stat_model.h"
#include "mem_model.h"
#include "net_model.h"
#include "mem_piechart.h"
#include "cpu_load_barchart.h"
#include "cpu_stat_barchart.h"
#include "gpu_model.h"

using namespace std;
namespace monitor {

class MonitorWidget : public QWidget {
    Q_OBJECT

   public:
    explicit MonitorWidget(QWidget* parent = nullptr);
    ~MonitorWidget(){};

    QWidget* ShowAllMonitorWidget(const string& name);
    QWidget* InitCpuMonitorWidget();
    QWidget* InitSoftIrqMonitorWidget();
    QWidget* InitMemMonitorWidget();
    QWidget* InitNetMonitorWidget();
    QWidget* InitGpuMonitorWidget();
    QWidget* InitButtonMenu(const string& name);

    void UpdateData(const monitor::proto::MonitorInfo& monitor_info);

   private slots:
    void ClickCpuButton();
    void ClickSoftIrqButton();
    void ClickMemButton();
    void ClickNetButton();
    void ClickGpuButton();

   private:
    QTableView* monitor_view_ = nullptr;
    QTableView* cpu_load_monitor_view_ = nullptr;
    QTableView* cpu_stat_monitor_view_ = nullptr;
    QTableView* mem_monitor_view_ = nullptr;
    QTableView* net_monitor_view_ = nullptr;
    QTableView* gpu_monitor_view_ = nullptr;

    MonitorBaseModel* monitor_model_ = nullptr;
    CpuLoadModel* cpu_load_model_ = nullptr;
    CpuStatModel* cpu_stat_model_ = nullptr;
    MemModel* mem_model_ = nullptr;
    NetModel* net_model_ = nullptr;
    GpuInfoModel* gpu_model_ = nullptr;
    MemPie* mem_pie = nullptr;
    CPUlaodBar* cpu_load_bar = nullptr;
    CPUstatBar* cpu_stat_bar = nullptr;

    QStackedLayout* stack_menu_ = nullptr;
};

}  // namespace monitor
#endif  // MONITOR_WIDGET_H
