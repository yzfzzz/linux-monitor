#include <QApplication>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "client/rpc_client.h"
#include "mprpcapplication.h"
#include "query_data.h"
#include "widget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QList<QPointF>>("QList<QPointF>");
    qRegisterMetaType<char **>("char**");
    Widget widget(argc, argv);
    widget.show();
    return app.exec();
}
