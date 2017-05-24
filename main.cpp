#include "widget.h"
#include <QApplication>
#include <QDir>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator *ts = new QTranslator;
    ts->load(QDir::currentPath()+"/app.qm");
    a.installTranslator(ts);
    Widget w;
    w.super_show();
    return a.exec();
}
