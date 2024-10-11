#include <random>
#include "cpu_stat_barchart.h"

static const QStringList list_bar_state = {
    "AVG_1",
    "AVG_3",
    "AVG_15",
};

QWidget* CPUstatBar::CPUstat_BarInit() {
    // QBarSeries 类将一系列数据显示为按类别分组的垂直条。
    this->series = new QBarSeries();
    // QBarSet 类表示条形图中的一组条形
    this->cpu_percent_set = new QBarSet("cpu");
    this->user_set = new QBarSet("user");
    this->sys_set = new QBarSet("system");

    *this->cpu_percent_set << 1 << 5 << 10;
    *this->user_set << 3 << 6 << 4;
    *this->sys_set << 9 << 7 << 2;

    this->series->append(this->cpu_percent_set);
    this->series->append(this->user_set);
    this->series->append(this->sys_set);

    this->chart = new QChart();  // 获取QChartView中默认的QChart

    this->chart->addSeries(this->series);  // 将创建好的条形图series添加进chart中
    // this->chart->setTitle("cpu current load rate");               // 设置标题
    this->chart->setAnimationOptions(QChart::SeriesAnimations);  // 设置图表变化时的动画效果

    QStringList
        x_label;  // X轴分类，一般与QBarSet中添加的数据个数相同，如果少了则会显示不全,多了不影响，但是不能重复
    x_label << "cpu"
            << "cpu0"
            << "cpu1";  

    QBarCategoryAxis* axisX = new QBarCategoryAxis();  // QBarCategoryAxis类向图表的轴添加类别。
    axisX->append(x_label);

    this->chart->addAxis(axisX, Qt::AlignBottom);  // 将X轴放在图表的底部
    this->series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();  // 创建Y轴
    axisY->setRange(0, 100);                // 设置Y轴范围
    // axisY->setTitleText("load ratio");   // 设置Y轴标题
    this->chart->addAxis(axisY, Qt::AlignLeft);  // Y轴左对齐
    this->series->attachAxis(axisY);

    this->chart->setTheme(QChart::ChartThemeBlueCerulean);
    this->chart->legend()->setAlignment(Qt::AlignRight);

    this->chartView = new QChartView(this->chart);
    this->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿渲染提示

    this->layout = new QVBoxLayout();
    this->layout->addWidget(this->chartView);  // 将QChartView添加到布局中

    QWidget* widget = new QWidget();
    widget->setLayout(this->layout);
    return widget;
}

void CPUstatBar::UpdateCPUstatBar(const monitor::proto::MonitorInfo& monitor_info) {
    for (int i = 0; i < monitor_info.cpu_stat_size(); i++) {
        const monitor::proto::CpuStat cpu_stat = monitor_info.cpu_stat(i);
        this->cpu_percent_set->replace(i, cpu_stat.cpu_percent());
        this->user_set->replace(i, cpu_stat.usr_percent());
        this->sys_set->replace(i, cpu_stat.system_percent());
    }
}
