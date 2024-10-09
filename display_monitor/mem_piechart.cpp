#include <random>
#include "mem_piechart.h"

//定义各扇形切片的颜色
static const QStringList list_pie_color = {
    "#A1DC85","#FF7777","#6480D6",  "#FFAD25",  "#84D1EF", "#4CB383",
};

static const QStringList list_pie_state = {
    "Free",
    "Used",
};

QWidget* MemPie::MemPie_Init()
{
    QWidget* widget = new QWidget();
    QVector<qreal> data(2, 50);
    series = new QPieSeries();  // 创建一个饼图对象（设置孔径就是圆环）
    for (int i = 0; i < data.size(); i++) {
        QPieSlice* pie_slice = new QPieSlice();
        pie_slice->setLabelVisible(true);
        pie_slice->setValue(data[i]);
        pie_slice->setLabel(list_pie_state[i]);
        pie_slice->setColor(list_pie_color[i]);
        pie_slice->setLabelColor(list_pie_color[i]);
        pie_slice->setBorderColor(list_pie_color[i]);
        series->append(pie_slice);
    }

    chart = new QChart();             // 获取QChartView中默认的QChart
    chart->setTitle("Memory Usage");  // 设置图表标题
    chart->setTheme(QChart::ChartThemeDark);  //设置暗黑主题
    chart->addSeries(series);  // 将创建好的饼图对象添加进QChart

    //图例
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(false);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿渲染提示

    layout = new QVBoxLayout();
    layout->addWidget(chartView);  // 将QChartView添加到布局中

    widget->setLayout(layout);
    return widget;
}

void MemPie::UpdateMemChart(
    const monitor::proto::MonitorInfo& monitor_info) {
    QVector<qreal> data;
    data.push_back(monitor_info.mem_info().used_percent());
    data.push_back(100 - monitor_info.mem_info().used_percent());

    // data.push_back(randomNumber);
    // data.push_back(100-randomNumber);
    for (int i = 0; i < data.size(); i++) {
        series->slices().at(i)->setValue(data[i]);
    }

    chart->update();  // 刷新图表
}
