#pragma once

#include <QWidget>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include <QtWidgets/QVBoxLayout>
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

class CPUlaod_BarWidget : public QWidget
{
    Q_OBJECT

public:
    QWidget* CPUlaod_BarInit();
    void UpdateCPUloadChart(const monitor::proto::MonitorInfo& monitor_info);

    QBarSeries* series;
    QChart* chart;
    QChartView *chartView;
    QVBoxLayout *layout;

    QBarSet* set1 = nullptr;
    QBarSet* set3 = nullptr;
    QBarSet* set15 = nullptr;

};

