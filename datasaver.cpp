#include "datasaver.h"
#include "QDebug"
DataSaver::DataSaver()
{
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
    csv = new Csv;
    excel = new ToExcel;
}

void DataSaver::writeCsv(QStringList s, QString id)
{
    qDebug()<<"ds called!";
    csv->addRow(s,id);
}

void DataSaver::readCsv(QString id)
{
    this->buff = csv->readAll(id);
    emit readyRead(this->buff);
}

void DataSaver::exportExcel(QStringList s)
{

}
