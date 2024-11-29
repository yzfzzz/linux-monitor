#pragma once
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QHBoxLayout>
#include <QList>
#include <QPointF>
#include <QSplineSeries>
#include <QWidget>
#include <iostream>
#include <string>

using namespace QtCharts;

class NetChart : public QWidget {
    Q_OBJECT

   public:
    explicit NetChart(QWidget *parent = 0);
    ~NetChart();

    // 接收到数据源发送来的数据，
    void drawChart(QList<QPointF> data_send, QList<QPointF> data_recv);

   private:
    int max_x_;
    int max_y_;

    QChart *chart_;
    QChartView *chart_view_;
    QSplineSeries *spline_series_send_;
    QSplineSeries *spline_series_recv_;

    QDateTimeAxis *axis_x_;
};
