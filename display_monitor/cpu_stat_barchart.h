#pragma once

#include <QWidget>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include <QtWidgets/QVBoxLayout>
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
// 0-100

class CPUstatBar {
   public:
    QBarSeries* series;
    QChart* chart;
    QChartView* chartView;
    QVBoxLayout* layout;


    QBarSet* cpu_percent_set = nullptr;
    QBarSet* user_set = nullptr;
    QBarSet* sys_set = nullptr;

    QWidget* CPUstat_BarInit();
    void UpdateCPUstatBar(const monitor::proto::MonitorInfo& monito_info);
};
