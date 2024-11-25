#include <QDateTime>
#include <QString>
#include <iostream>
#include <string>
#include "ui_widget.h"
#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // ui->water1->setValue(70);
    // ui->water2->setValue(90);
    // ui->water3->setValue(20);
    // ui->water4->setValue(50);

    // ui->gpu_name->setText("显卡型号\nNVIDIA MX250");
    // ui->gpu_num->setText("显卡数量\n6");

    // ui->label_up_1->setText("CPU使用率");
    // ui->label_down_1->setText("运行流畅");

    // ui->label_up_2->setText("运行内存使用率");
    // ui->label_down_2->setText("6/16(GB)");
}

void Widget::Update(std::vector<monitor::MidInfo> midinfo_array) {
    if(midinfo_array.empty())
    {
        return;
    }
    monitor::MidInfo midinfo = midinfo_array[midinfo_array.size() - 1];
    std::string gpu_name = "显卡型号\n" + midinfo.gpu_name;
    ui->gpu_name->setText(QString::fromStdString(gpu_name));
    std::string gpu_num = "显卡数量\n" + std::to_string(midinfo.gpu_num);
    ui->gpu_num->setText(QString::fromStdString(gpu_num));

    // GPU使用率
    ui->label_up_1->setText("显卡使用率");
    ui->water1->setValue(midinfo.gpu_avg_util);
    std::string label_down_1_str;
    if (midinfo.gpu_avg_util > 70) {
        label_down_1_str = "繁忙";
        ui->label_down_1->setStyleSheet("QLabel{color:red;}");
    } else {
        label_down_1_str = "流畅";
        ui->label_down_1->setStyleSheet("QLabel{color:green;}");
    }
    ui->label_down_1->setText(QString::fromStdString(label_down_1_str));

    // 显存使用情况
    ui->label_up_2->setText("显卡内存");
    std::cout << "gpu_used_mem: " << midinfo.gpu_used_mem << " | "
              << "gpu_total_mem: " << midinfo.gpu_total_mem << " | "
              << "res: "
              << (int)(midinfo.gpu_used_mem * 100 / midinfo.gpu_total_mem)
              << std::endl;
    ui->water2->setValue(
        (int)(midinfo.gpu_used_mem * 100 / midinfo.gpu_total_mem));
    std::string label_down_2_str = std::to_string(midinfo.gpu_used_mem) + "/" +
                                   std::to_string(midinfo.gpu_total_mem) +
                                   "(M)";
    ui->label_down_2->setText(QString::fromStdString(label_down_2_str));

    // CPU
    ui->label_up_3->setText("CPU使用率");
    ui->water3->setValue(midinfo.cpu_load_avg_1 * 10);
    std::string label_down_3_str;
    if (midinfo.cpu_load_avg_3 * 10 > 70) {
        label_down_3_str = "繁忙";
        ui->label_down_3->setStyleSheet("QLabel{color:red;}");
    } else {
        label_down_3_str = "流畅";
        ui->label_down_3->setStyleSheet("QLabel{color:green;}");
    }
    ui->label_down_3->setText(QString::fromStdString(label_down_3_str));

    // 运行内存
    ui->label_up_4->setText("运行内存");
    int total_mem = midinfo.mem_total;
    int used_mem = midinfo.mem_used * midinfo.mem_total * 0.01;
    ui->water4->setValue(midinfo.mem_used);
    std::string label_down_4_str =
        std::to_string(used_mem) + "/" + std::to_string(total_mem) + "(G)";
    ui->label_down_4->setText(QString::fromStdString(label_down_4_str));

    for (int i = 0; i < midinfo_array.size(); i++) {
        // GPU使用情况
        ui->schart_left->dataReceived(midinfo_array[i].gpu_avg_util);

        //网络收发
        std::cout << "[" << i << "]  " << midinfo_array[i].timehms << ":  "
                  << midinfo_array[i].net_rcv_rate << std::endl;
        ui->schart_right->dataReceived(midinfo_array[i].net_send_rate,
                                       midinfo_array[i].net_rcv_rate,
                                       midinfo_array[i].timehms);
    }
    std::cout << "-----------" << std::endl;
    ui->schart_right->drawChart();
    ui->schart_left->drawChart();
}

Widget::~Widget() { delete ui; }
