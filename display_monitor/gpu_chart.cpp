#include <QDateTime>
#include <QHBoxLayout>
#include "gpu_chart.h"

GPUChart::GPUChart(QWidget *parent) : QWidget(parent) {
    max_y_ = 100;

    spline_series_ = new QSplineSeries();
    QPen line_pen(QColor("#BB30E2"), 2, Qt::SolidLine, Qt::RoundCap,
                 Qt::RoundJoin);
    spline_series_->setPen(line_pen);

    chart_ = new QChart();
    chart_->addSeries(spline_series_);

    chart_->legend()->hide();
    chart_->setTitle("GPU平均使用率");
    chart_->createDefaultAxes();

    axis_x_ = new QDateTimeAxis();
    axis_x_->setFormat("mm:ss");
    axis_x_->setTickCount(5);

    if (chart_->axisX()) {
        chart_->removeAxis(chart_->axisX());
    }

    chart_->setAxisX(axis_x_);
    spline_series_->attachAxis(axis_x_);

    chart_->axisY()->setRange(0, max_y_);
    chart_->setBackgroundVisible(false);

    if (chart_->axisX()) {
        chart_->axisX()->setGridLineVisible(false);  // 隐藏X轴网格线
    }
    if (chart_->axisY()) {
        chart_->axisY()->setGridLineVisible(false);  // 隐藏Y轴网格线
    }

    chart_view_ = new QChartView(chart_);
    chart_view_->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chart_view_);
    setLayout(layout);
}

GPUChart::~GPUChart() {}

void GPUChart::drawChart(QList<QPointF> gpu_data) {
    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        spline_series_->replace(gpu_data);
        axis_x_->setRange(QDateTime::fromMSecsSinceEpoch(gpu_data.begin()->x()),
                        QDateTime::fromMSecsSinceEpoch(gpu_data.rbegin()->x()));
    }
}
