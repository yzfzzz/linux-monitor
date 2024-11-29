#pragma once
#include <QList>
#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QSplineSeries>
#include <string>

using namespace QtCharts;

class GPUChart : public QWidget {
    Q_OBJECT

   public:
    explicit GPUChart(QWidget *parent = 0);
    ~GPUChart();
    void drawChart(QList<QPointF> data);

   private:
    int max_x_;
    int max_y_;

    QChart *chart_;
    QChartView *chart_view_;
    QSplineSeries *spline_series_;
    QDateTimeAxis *axis_x_;
};