#ifndef DATASAVER_H
#define DATASAVER_H

#include <QObject>
#include <QThread>
#include <toexcel.h>
#include <csv.h>
#include <widget.h>

class DataSaver:public QObject
{
   Q_OBJECT
public:
    DataSaver();
    QStringList buff;

private:
    QThread *thread;
    Csv *csv;
    ToExcel *excel;

signals:
    void readyRead(QStringList s);

public slots:
    void writeCsv(QStringList s,QString id);
    void readCsv(QString id);
    void exportExcel(QStringList s);
};

#endif // DATASAVER_H
