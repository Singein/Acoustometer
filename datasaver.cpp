#include "datasaver.h"
#include "QDebug"
DataSaver::DataSaver()
{
    csv = new Csv;
    excel = new Excel;
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
    excel->setCsv(csv);
}

void DataSaver::writeCsv(QStringList s, QString id)
{
    csv->addRow(s,id);
}

void DataSaver::readCsv(QString id)
{
    this->buff = csv->readAll(id);
    emit readyRead(this->buff);
}

void DataSaver::exportExcel(QStringList s,QString fileName)
{
    excel->Export(fileName,s);
}
