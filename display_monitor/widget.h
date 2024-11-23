#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "water_progress.h"
#include "midinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void Update(monitor::MidInfo midinfo);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
