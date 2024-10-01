#include "cpu_load_model.h"


namespace monitor {

CpuLoadModel::CpuLoadModel(QObject* parent): MonitorInterModel(parent){
    header_ << tr("load_1");
    header_ << tr("load_3");
    header_ << tr("load_15");
}

int CpuLoadModel::rowCount(const QModelIndex &parent) const{
    return  monitor_data_.size();
}

int CpuLoadModel::columnCount(const QModelIndex &parent) const{
    return COLUMN_MAX;
}

// 获取表头的名称
QVariant CpuLoadModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        return header_[section];
    }
    return MonitorInterModel::headerData(section, orientation, role);
}


// 获取index=(x,y)的数据
QVariant CpuLoadModel::data(const QModelIndex &index, int role) const{
    if(index.column() < 0 || index.column() >= COLUMN_MAX)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.row() < monitor_data_.size() && index.column() < COLUMN_MAX){
            return monitor_data_[index.row()][index.column()];
        }
    }
    return QVariant();
}

// 更新信息
void CpuLoadModel::UpdateMonitorInfo(const monitor::proto::MonitorInfo& monitor_info){
    beginResetModel();
    monitor_data_.clear();
    monitor_data_.push_back(insert_one_cou_load(monitor_info.cpu_load()));
    endResetModel();
    return;
}


vector<QVariant> CpuLoadModel::insert_one_cou_load(const monitor::proto::CpuLoad &cpu_load){
    vector<QVariant> cpu_load_list;

    for(int i = CpuLoad::CPU_AVG_1; i < COLUMN_MAX; i++)
    {
        switch (i) {
            case CpuLoad::CPU_AVG_1:
                cpu_load_list.push_back(QVariant(cpu_load.load_avg_1()));
            break;

            case CpuLoad::CPU_AVG_3:
                cpu_load_list.push_back(QVariant(cpu_load.load_avg_3()));
            break;

            case CpuLoad::CPU_AVG_15:
                cpu_load_list.push_back(QVariant(cpu_load.load_avg_15()));
            break;

        }
    }
    return cpu_load_list;
}




















}
