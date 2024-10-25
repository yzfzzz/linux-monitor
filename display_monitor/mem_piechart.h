#pragma once

#include <QPieSeries>
#include <QWidget>
QT_CHARTS_USE_NAMESPACE
#include <QtCharts>
#include <QtWidgets/QVBoxLayout>
#include "monitor_info.pb.h"

class MemPie
{
   public:

    void UpdateMemChart(const monitor::proto::MonitorInfo& monitor_info);
    QWidget* MemPie_Init();
    QPieSeries* series;
    QChart* chart;
    QChartView* chartView;
    QVBoxLayout* layout;
};
