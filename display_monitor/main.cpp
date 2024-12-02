#include <QApplication>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include "start.h"
#include "mprpcapplication.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MprpcApplication::Init(argc, argv);
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QList<QPointF>>("QList<QPointF>");
    qRegisterMetaType<char **>("char**");
    qRegisterMetaType<std::string>("std::string");

    QFile file(":/resource/qss/style-1.qss"); /*QSS文件所在的路径*/
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    app.setStyleSheet(stylesheet);
    file.close();

    Start s(argc, argv);
    s.show();

    return app.exec();
}
