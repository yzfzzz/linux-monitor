#include <QDateTime>
#include <QHBoxLayout>
#include "smooth_chart.h"

smooth_chart::smooth_chart(QWidget *parent) : QWidget(parent) {
    maxSize = 30;  // 只存储最新的 30 个数据
    maxY = 100;

    splineSeries = new QSplineSeries();
    QPen linePen(QColor("#BB30E2"), 2, Qt::SolidLine, Qt::RoundCap,
                 Qt::RoundJoin);
    splineSeries->setPen(linePen);

    chart = new QChart();
    chart->addSeries(splineSeries);

    chart->legend()->hide();
    chart->setTitle("GPU平均使用率");
    chart->createDefaultAxes();

    axisX = new QDateTimeAxis();
    axisX->setFormat("mm:ss");
    axisX->setTickCount(5);
    
    if (chart->axisX()) {
        chart->removeAxis(chart->axisX());
    }

    chart->setAxisX(axisX);
    splineSeries->attachAxis(axisX);

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

smooth_chart::~smooth_chart() {}

void smooth_chart::dataReceived(int value, std::string cur_time) {
    QString timeStr = QString::fromStdString(cur_time);

    QDateTime time = QDateTime::fromString(timeStr, "hh:mm:ss");

    gpu_line_node gpu_node;
    gpu_node.value = value;
    gpu_node.time = time;

    data << gpu_node;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (data.size() > maxSize) {
        data.removeFirst();
    }
}

void smooth_chart::drawChart() {
    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        splineSeries->clear();

        for (int i = 0; i < data.size(); ++i) {
            splineSeries->append(data.at(i).time.toMSecsSinceEpoch(),
                                 data.at(i).value);
        }
        axisX->setRange(data.begin()->time, data.rbegin()->time);
    }
}
