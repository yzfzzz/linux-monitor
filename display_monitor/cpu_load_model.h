#ifndef CPU_LOAD_MODEL_H
#define CPU_LOAD_MODEL_H

#include <vector>
#include "monitor_inter.h"
using namespace std;
namespace monitor {
class CpuLoadModel : public MonitorInterModel
{
    Q_OBJECT
public:
    explicit CpuLoadModel(QObject* parent = nullptr);
    virtual ~CpuLoadModel() {}
    // ?const QModelIndex &parent = QModelIndex()
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void UpdateMonitorInfo(const monitor::proto::MonitorInfo& monitor_info);

signals:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex& bottomRight, const QVector<int> &roles);

private:
    vector<QVariant> insert_one_cou_load(const monitor::proto::CpuLoad &cpu_load);
    vector<vector<QVariant>> monitor_data_;
    QStringList header_;

    enum CpuLoad{
        CPU_AVG_1 = 0,
        CPU_AVG_3,
        CPU_AVG_15,
        COLUMN_MAX
    };
};

}

#endif // CPU_LOAD_MODEL_H
