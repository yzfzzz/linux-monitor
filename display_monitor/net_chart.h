#ifndef net_chart_H
#define net_chart_H

#include <QWidget>
#include <QList>

#include <QSplineSeries>
#include <QScatterSeries>
#include <QAreaSeries>
#include <QChart>
#include <QChartView>

using namespace QtCharts;

class net_chart : public QWidget {
    Q_OBJECT

public:
    explicit net_chart(QWidget *parent = 0);
    ~net_chart();
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value_send, int value_recv);

private:

    int maxSize;  // data 最多存储 maxSize 个元素
    int maxX;
    int maxY;
    QList<double> data_send; // 存储业务数据的 list
    QList<double> data_recv;

    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries_send;
    QSplineSeries *splineSeries_recv;
};

#endif // net_chart_H
