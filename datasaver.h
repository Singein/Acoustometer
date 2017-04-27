#ifndef DATASAVER_H
#define DATASAVER_H

#include <QObject>
#include <QThread>
#include <excel.h>
#include <csv.h>
#include <widget.h>

class DataSaver:public QObject
{
   Q_OBJECT
public:
    DataSaver();
    QStringList buff;
    Excel *excel;

private:
    QThread *thread;
    Csv *csv;


signals:
    void readyRead(QStringList s);

public slots:
    void writeCsv(QStringList s,QString id);
    void readCsv(QString id);
    void exportExcel(QStringList s,QString filename);
};

#endif // DATASAVER_H
