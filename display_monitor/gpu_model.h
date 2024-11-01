#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "monitor_info.pb.h"
#include "monitor_inter.h"

namespace monitor {

class GpuInfoModel : public MonitorInterModel {
    Q_OBJECT

   public:
    explicit GpuInfoModel(QObject *parent = nullptr);

    virtual ~GpuInfoModel() {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void UpdateMonitorInfo(const monitor::proto::MonitorInfo &monitor_info);

   signals:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QVector<int> &roles);

   private:
    std::vector<QVariant> insert_one_gpu_info(const monitor::proto::GpuInfo &gpu_info);
    std::vector<std::vector<QVariant>> monitor_data_;
    QStringList header_;

    enum GpuParam {
        ID = 0,
        GPU_NAME,
        GPU_MEM_TOTAL,
        GPU_MEM_FREE,
        GPU_MEM_USED,
        GPU_MEM_UTILIZE,
        GPU_UTILIZE,
        TEMPERTURE,
        FAN_SPEED,
        POWER_STAT,
        COLUMN_MAX
    };
};

}  // namespace monitor

