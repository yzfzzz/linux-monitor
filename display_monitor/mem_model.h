#ifndef MEM_MODEL_H
#define MEM_MODEL_H
#include <QAbstractTableModel>
#include <vector>
#include "monitor_info.pb.h"
#include "monitor_inter.h"

using namespace std;
namespace monitor {
class MemModel : public MonitorInterModel {
    Q_OBJECT

   public:
    explicit MemModel(QObject *parent = nullptr);
    virtual ~MemModel() {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    void UpdateMonitorInfo(const monitor::proto::MonitorInfo &monitor_info);

   signals:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QVector<int> &roles);

   private:
    vector<QVariant> insert_one_mem_info(
        const monitor::proto::MemInfo &mem_info);
    vector<vector<QVariant>> monitor_data_;
    QStringList header_;

    enum MemInfo {
        USED_PERCENT = 0,
        TOTAL,
        FREE,
        AVAIL,
        BUFFERS,
        CACHED,
        SWAP_CACHED,
        ACTIVE,
        INACTIVE,
        ACTIVE_ANON,
        INACTIVE_ANON,
        DIRTY,
        WRITEBACK,
        ANON_PAGES,
        MAPPED,
        KRECLAIMABLE,
        SRECLAIMABLE,
        SUNRECLAIM,
        COLUMN_MAX
    };
};
}  // namespace monitor

#endif  // MEM_MODEL_H
