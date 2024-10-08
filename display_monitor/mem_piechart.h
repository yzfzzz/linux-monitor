#pragma once

#include <QPieSeries>
#include <QWidget>
QT_CHARTS_USE_NAMESPACE
#include <QtCharts>
#include <QtWidgets/QVBoxLayout>
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

class PieWidget : public QWidget {
    Q_OBJECT

   public:
    PieWidget(QWidget* parent = nullptr);
    ~PieWidget(){};

    void UpdateMemChart(const monitor::proto::MonitorInfo& monitor_info);

    QPieSeries* series;
    QChart* chart;
    QChartView* chartView;
    QVBoxLayout* layout;
};
