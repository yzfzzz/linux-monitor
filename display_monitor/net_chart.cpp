#include <QDateTime>
#include <QHBoxLayout>
#include "net_chart.h"


net_chart::net_chart(QWidget *parent) : QWidget(parent) {
    maxSize = 30;  // 只存储最新的 30 个数据
    maxX = 300;
    maxY = 200;

    splineSeries_send = new QSplineSeries();
    QPen linePen_send(QColor("#05D0D7"), 2, Qt::SolidLine, Qt::RoundCap,
                      Qt::RoundJoin);
    splineSeries_send->setPen(linePen_send);

    splineSeries_recv = new QSplineSeries();
    QPen linePen_recv(QColor("#DB901E"), 2, Qt::SolidLine, Qt::RoundCap,
                      Qt::RoundJoin);
    splineSeries_recv->setPen(linePen_recv);

    chart = new QChart();
    chart->addSeries(splineSeries_send);
    chart->addSeries(splineSeries_recv);

    chart->legend()->hide();
    chart->setTitle("流量收发情况");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 200);
    chart->axisY()->setRange(0, maxY);
    chart->setBackgroundVisible(false);

    if (chart->axisX()) {
        chart->axisX()->setGridLineVisible(false);  // 隐藏X轴网格线
    }
    if (chart->axisY()) {
        chart->axisY()->setGridLineVisible(false);  // 隐藏Y轴网格线
    }

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

net_chart::~net_chart() {}


void net_chart::dataReceived(int value_send, int value_recv) {
    data_send << value_send;
    data_recv << value_recv;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data_send.size() > maxSize) {
        data_send.removeFirst();
    }
    while (data_recv.size() > maxSize) {
        data_recv.removeFirst();
    }

    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        splineSeries_send->clear();
        splineSeries_recv->clear();
        int dx = maxX / (maxSize - 1);
        int less = maxSize - data_send.size();

        for (int i = 0; i < data_send.size(); ++i) {
            splineSeries_send->append(less * dx + i * dx, data_send.at(i));
            splineSeries_recv->append(less * dx + i * dx, data_recv.at(i));
        }
    }
}
