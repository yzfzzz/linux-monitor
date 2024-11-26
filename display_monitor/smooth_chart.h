#ifndef smooth_chart_H
#define smooth_chart_H

#include <QList>
#include <QWidget>


#include <QAreaSeries>
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QScatterSeries>
#include <QSplineSeries>
#include <string>


using namespace QtCharts;

class gpu_line_node {
   public:
    int value;
    QDateTime time;
};

class smooth_chart : public QWidget {
    Q_OBJECT

   public:
    explicit smooth_chart(QWidget *parent = 0);
    ~smooth_chart();
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value, std::string cur_time);
    void drawChart();

   private:
    int maxSize;  // data 最多存储 maxSize 个元素
    int maxX;
    int maxY;
    QList<gpu_line_node> data;  // 存储业务数据的 list

    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;
    QDateTimeAxis *axisX;
};

#endif  // smooth_chart_H
