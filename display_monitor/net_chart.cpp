#include <QHBoxLayout>
#include "net_chart.h"

net_chart::net_chart(QWidget *parent) : QWidget(parent) {
    maxSize = 30;  // 只存储最新的 30 个数据
    maxX = 200;
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

    axisX = new QDateTimeAxis();
    axisX->setFormat("mm:ss");
    axisX->setTickCount(5);
    axisX->setLabelsAngle(45);

    if (chart->axisX()) {
        chart->removeAxis(chart->axisX());
    }

    chart->setAxisX(axisX);
    splineSeries_send->attachAxis(axisX);
    splineSeries_recv->attachAxis(axisX);

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

void net_chart::dataReceived(int value_send, int value_recv,
                             std::string cur_time) {
    std::cout << cur_time << std::endl;
    QString timeStr = QString::fromStdString(cur_time);

    QDateTime time = QDateTime::fromString(timeStr, "hh:mm:ss");

    line_node send_node;
    send_node.value = value_send;
    send_node.time = time;

    line_node recv_node;
    recv_node.value = value_recv;
    recv_node.time = time;

    data_send << send_node;
    data_recv << recv_node;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data_send.size() > maxSize) {
        data_send.removeFirst();
    }
    while (data_recv.size() > maxSize) {
        data_recv.removeFirst();
    }
}

void net_chart::drawChart() {
    if (isVisible()) {
        splineSeries_send->clear();
        splineSeries_recv->clear();
        int dx = maxX / (maxSize - 1);
        int less = maxSize - data_send.size();

        for (int i = 0; i < data_send.size(); ++i) {
            std::cout << "["<< i << "]  "<< data_send.at(i).time.toString("hh:mm:ss").toStdString() << ":  "<< data_send.at(i).value << std::endl;
            splineSeries_send->append(data_send.at(i).time.toMSecsSinceEpoch(),
                                      data_send.at(i).value);
            splineSeries_recv->append(data_recv.at(i).time.toMSecsSinceEpoch(),
                                      data_recv.at(i).value);
        }
        axisX->setRange(data_send.begin()->time, data_send.rbegin()->time);
    }
}
