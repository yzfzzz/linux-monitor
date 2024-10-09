#pragma once

#include <QWidget>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include <QtWidgets/QVBoxLayout>
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

class CPUlaodBar {
   public:
    QHorizontalBarSeries* series;
    QChart* chart;
    QChartView* chartView;
    QVBoxLayout* layout;

    QBarSet* set1 = nullptr;
    QBarSet* set3 = nullptr;
    QBarSet* set15 = nullptr;

    QWidget* CPUlaod_BarInit();
    void UpdateCPUloadBar(const monitor::proto::MonitorInfo& monito_info);
};
