#include "cpu_load_barchart.h"
#include <random>


static const QStringList list_bar_state = {
"AVG_1", "AVG_3", "AVG_15",
};

QWidget* CPUlaod_BarWidget::CPUlaod_BarInit()
{
    // QBarSeries 类将一系列数据显示为按类别分组的垂直条。
    series = new QBarSeries();
       // QBarSet 类表示条形图中的一组条形
    set1 = new QBarSet("AVG_1");
    set3 = new QBarSet("AVG_3");
    set15 = new QBarSet("AVG_15");

    *set1 << 0.1;
    *set3 << 0.3;
    *set15 << 0.8;

    series->append(set1);
    series->append(set3);
    series->append(set15);


    chart = new QChart();           // 获取QChartView中默认的QChart

    chart->addSeries(series);                              // 将创建好的条形图series添加进chart中
    chart->setTitle("Short-term load rate");               // 设置标题
    chart->setAnimationOptions(QChart::SeriesAnimations);  // 设置图表变化时的动画效果

    QBarCategoryAxis* axisX = new QBarCategoryAxis();      // QBarCategoryAxis类向图表的轴添加类别。
    chart->addAxis(axisX, Qt::AlignBottom);                // 将X轴放在图表的底部
    series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();     // 创建Y轴
    axisY->setRange(0, 1);                   // 设置Y轴范围（学习啊，当然是24小时喽）
    axisY->setTitleText("Average load ratio");  // 设置Y轴标题
    chart->addAxis(axisY, Qt::AlignLeft);     // Y轴左对齐
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // 设置抗锯齿渲染提示

    layout = new QVBoxLayout();
    layout->addWidget(chartView); // 将QChartView添加到布局中
    
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

void CPUlaod_BarWidget::UpdateCPUloadChart(const monitor::proto::MonitorInfo& monitor_info)
{
    set1->replace(0, monitor_info.cpu_load().load_avg_1());
    set3->replace(0, monitor_info.cpu_load().load_avg_3());
    set15->replace(0, monitor_info.cpu_load().load_avg_15());
    series->clear();
    series->append(set1);
    series->append(set3);
    series->append(set15);

    // chart->update();  // 刷新图表
}





