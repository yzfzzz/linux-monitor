#include "smooth_chart.h"
#include <QDateTime>
#include <QHBoxLayout>

smooth_chart::smooth_chart(QWidget *parent) : QWidget(parent) {
    maxSize = 11; // 只存储最新的 31 个数据
    maxX = 300;
    maxY = 100;

    splineSeries = new QSplineSeries();
    QPen linePen(QColor("#BB30E2"),2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    splineSeries->setPen(linePen);

    chart = new QChart();
    chart->addSeries(splineSeries);

    chart->legend()->hide();
    chart->setTitle("GPU平均使用率");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 300);
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

smooth_chart::~smooth_chart() {
}

void smooth_chart::dataReceived(int value) {
    data << value;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data.size() > maxSize) {
        data.removeFirst();
    }

    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        splineSeries->clear();
        int dx = maxX / (maxSize-1);
        int less = maxSize - data.size();

        for (int i = 0; i < data.size(); ++i) {
            splineSeries->append(less*dx+i*dx, data.at(i));
        }
    }
}
