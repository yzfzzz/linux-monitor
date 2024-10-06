#include "mem_piechart.h"
#include <random>

//定义各扇形切片的颜色
static const QStringList list_pie_color = {
    "#6480D6","#A1DC85","#FFAD25","#FF7777","#84D1EF","#4CB383",
};

PieWidget::PieWidget(QWidget *parent)
    : QWidget(parent)
    
{
    QVector<qreal> data(2,50);
    series = new QPieSeries(this);            // 创建一个饼图对象（设置孔径就是圆环）
    for(int i = 0; i < data.size(); i++)
    {
        QPieSlice* pie_slice = new QPieSlice(this);
        pie_slice->setLabelVisible(true);
        pie_slice->setValue(data[i]);
        pie_slice->setLabel(QString::number(data[i]));
        pie_slice->setColor(list_pie_color[i]);
        pie_slice->setLabelColor(list_pie_color[i]);
        pie_slice->setBorderColor(list_pie_color[i]);
        series->append(pie_slice);
    }

    chart = new QChart();           // 获取QChartView中默认的QChart
    chart->setTitle("used percent");                      // 设置图表标题
    chart->setTheme(QChart::ChartThemeDark);        //设置暗黑主题
    chart->addSeries(series);                        // 将创建好的饼图对象添加进QChart

    //图例
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(false);


    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // 设置抗锯齿渲染提示

    layout = new QVBoxLayout(this);
    layout->addWidget(chartView); // 将QChartView添加到布局中

}

void PieWidget::UpdateMemChart(const monitor::proto::MonitorInfo& monitor_info)
{
    QVector<qreal> data;
    // data.push_back(monitor_info.mem_info().used_percent());
    // data.push_back(100-monitor_info.mem_info().used_percent());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 100.0); // 生成 0 到 100 之间的随机浮点数

    float randomNumber = dis(gen);
    data.push_back(randomNumber);
    data.push_back(100-randomNumber);
    for(int i = 0; i < data.size(); i++)
    {
        series->slices().at(i)->setValue(data[i]);
    }

    chart->update();  // 刷新图表
}


