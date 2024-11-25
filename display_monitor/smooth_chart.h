#ifndef smooth_chart_H
#define smooth_chart_H

#include <QWidget>
#include <QList>

#include <QSplineSeries>
#include <QScatterSeries>
#include <QAreaSeries>
#include <QChart>
#include <QChartView>

using namespace QtCharts;

class smooth_chart : public QWidget {
    Q_OBJECT

public:
    explicit smooth_chart(QWidget *parent = 0);
    ~smooth_chart();
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value);
    void drawChart();

private:

    int maxSize;  // data 最多存储 maxSize 个元素
    int maxX;
    int maxY;
    QList<double> data; // 存储业务数据的 list

    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;
};

#endif // smooth_chart_H
