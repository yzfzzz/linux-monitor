#include <random>
#include "cpu_load_barchart.h"

static const QStringList list_bar_state = {
    "AVG_1",
    "AVG_3",
    "AVG_15",
};

QWidget* CPUlaodBar::CPUlaod_BarInit(){
    // QBarSeries 类将一系列数据显示为按类别分组的垂直条。
    this->series = new QBarSeries();
    // QBarSet 类表示条形图中的一组条形
    this->set1 = new QBarSet("AVG_1");
    this->set3 = new QBarSet("AVG_3");
    this->set15 = new QBarSet("AVG_15");

    *this->set1 << 0.1;
    *this->set3 << 0.3;
    *this->set15 << 0.8;

    this->series->append(this->set1);
    this->series->append(this->set3);
    this->series->append(this->set15);

    this->chart = new QChart();  // 获取QChartView中默认的QChart

    this->chart->addSeries(
        this->series);  // 将创建好的条形图series添加进chart中
    this->chart->setTitle("Short-term load rate");  // 设置标题
    this->chart->setAnimationOptions(
        QChart::SeriesAnimations);  // 设置图表变化时的动画效果

    QBarCategoryAxis* axisX =
        new QBarCategoryAxis();  // QBarCategoryAxis类向图表的轴添加类别。
    this->chart->addAxis(axisX, Qt::AlignBottom);  // 将X轴放在图表的底部
    this->series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();  // 创建Y轴
    axisY->setRange(0, 1);  // 设置Y轴范围（学习啊，当然是24小时喽）
    axisY->setTitleText("Average load ratio");   // 设置Y轴标题
    this->chart->addAxis(axisY, Qt::AlignLeft);  // Y轴左对齐
    this->series->attachAxis(axisY);

    this->chartView = new QChartView(this->chart);
    this->chartView->setRenderHint(
        QPainter::Antialiasing);  // 设置抗锯齿渲染提示

    this->layout = new QVBoxLayout();
    this->layout->addWidget(this->chartView);  // 将QChartView添加到布局中

    QWidget* widget = new QWidget();
    widget->setLayout(this->layout);
    return widget;
}

void CPUlaodBar::UpdateCPUloadBar(const monitor::proto::MonitorInfo &monito_info)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0); // 生成 0 到 100之间的随机浮点数 float randomNumber = dis(gen);

    this->set1->replace(0,dis(gen));
    this->set3->replace(0,dis(gen));
    this->set15->replace(0,dis(gen));
}
