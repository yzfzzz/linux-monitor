#include "gpu_model.h"

namespace monitor {
GpuInfoModel::GpuInfoModel(QObject *parent) : MonitorInterModel(parent) {
    header_ << tr("id");
    header_ << tr("name");
    header_ << tr("mem_total");
    header_ << tr("mem_free");
    header_ << tr("mem_used");
    header_ << tr("mem_utilize");
    header_ << tr("gpu_utilize");
    header_ << tr("tem");
    header_ << tr("fan");
    header_ << tr("power");
}

int GpuInfoModel::rowCount(const QModelIndex &parent) const { return monitor_data_.size(); }

int GpuInfoModel::columnCount(const QModelIndex &parent) const { return COLUMN_MAX; }

QVariant GpuInfoModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return header_[section];
    }

    return MonitorInterModel::headerData(section, orientation, role);
}

QVariant GpuInfoModel::data(const QModelIndex &index, int role) const {
    if (index.column() < 0 || index.column() >= COLUMN_MAX) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (index.row() < monitor_data_.size() && index.column() < COLUMN_MAX) {
            return monitor_data_[index.row()][index.column()];
        }
        return QVariant();
    }
}

void GpuInfoModel::UpdateMonitorInfo(const monitor::proto::MonitorInfo &monitor_info) {
    beginResetModel();
    monitor_data_.clear();

    for (int i = 0; i < monitor_info.gpu_info_size(); i++) {
        // std::cout <<monitor_info.gpu_info(i).cpu_name()<<std::endl;
        monitor_data_.push_back(insert_one_gpu_info(monitor_info.gpu_info(i)));
    }
    // QModelIndex leftTop = createIndex(0, 0);
    // QModelIndex rightBottom = createIndex(monitor_data_.size(), COLUMN_MAX);
    // emit dataChanged(leftTop, rightBottom, {});

    endResetModel();

    return;
}

std::vector<QVariant> GpuInfoModel::insert_one_gpu_info(const monitor::proto::GpuInfo &gpu_info) {
    std::vector<QVariant> gpu_info_list;
    for (int i = GpuParam::ID; i < COLUMN_MAX; i++) {
        switch (i) {
            case GpuParam::ID:
                gpu_info_list.push_back(QString::fromStdString(gpu_info.id()));
                break;
            case GpuParam::GPU_NAME:
                gpu_info_list.push_back(QString::fromStdString(gpu_info.gpu_name()));
                break;
            case GpuParam::GPU_MEM_TOTAL:
                gpu_info_list.push_back(QVariant(gpu_info.gpu_mem_total()));
                break;
            case GpuParam::GPU_MEM_FREE:
                gpu_info_list.push_back(QVariant(gpu_info.gpu_mem_free()));
                break;
            case GpuParam::GPU_MEM_USED:
                gpu_info_list.push_back(QVariant(gpu_info.gpu_mem_used()));
                break;
            case GpuParam::GPU_MEM_UTILIZE:
                gpu_info_list.push_back(QVariant(gpu_info.gpu_mem_utilize()));
                break;
            case GpuParam::GPU_UTILIZE:
                gpu_info_list.push_back(QVariant(gpu_info.gpu_utilize()));
                break;
            case GpuParam::TEMPERTURE:
                gpu_info_list.push_back(QVariant(gpu_info.temperture()));
                break;
            case GpuParam::FAN_SPEED:
                gpu_info_list.push_back(QVariant(gpu_info.fan_speed()));
                break;
            case GpuParam::POWER_STAT:
                gpu_info_list.push_back(QVariant(gpu_info.power_stat()));
                break;
            default:
                break;
        }
    }
    return gpu_info_list;
}
}  // namespace monitor
// namespace monitor
