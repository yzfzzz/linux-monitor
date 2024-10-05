#ifndef CPU_SOFTIRQ_MODEL_H
#define CPU_SOFTIRQ_MODEL_H
#include <QAbstractTableModel>
#include <vector>
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
#include "monitor_inter.h"

using namespace std;
namespace monitor {
class MonitorBaseModel : public MonitorInterModel {
    Q_OBJECT
   public:
    explicit MonitorBaseModel(QObject* parent = nullptr);
    virtual ~MonitorBaseModel() {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    void UpdateMonitorInfo(const monitor::proto::MonitorInfo& monito_info);

   signals:
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight,
                     const QVector<int>& roles);

   private:
    vector<QVariant> insert_one_soft_irq(
        const monitor::proto::SoftIrq& soft_irq);
    vector<vector<QVariant>> monitor_data_;
    QStringList header_;

    enum SoftIrqInfo {
        CPU_NAME = 0,
        HI,
        TIMER,
        NET_TX,
        NET_RX,
        BLOCK,
        IRQ_POLL,
        TASKLET,
        SCHED,
        HRTIMER,
        RCU,
        COLUMN_MAX
    };
};
}  // namespace monitor

#endif  // CPU_SOFTIRQ_MODEL_H
