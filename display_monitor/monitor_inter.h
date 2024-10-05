#ifndef MONITOR_INTERH_H
#define MONITOR_INTERH_H

#include <QAbstractTableModel>
#include <QObject>

namespace monitor {
class MonitorInterModel : public QAbstractTableModel {
    //    类定义中添加Q_OBJECT宏，这个类才能使用信号与槽机制
    Q_OBJECT;

   public:
    explicit MonitorInterModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}
    //使用了override描述符，那么该函数必须重载其基类中的同名函数
    // 获取index索引的数据
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    // 获取表头数据
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    virtual ~MonitorInterModel() {}
};
}  // namespace monitor

#endif  // MONITOR_INTERH_H
