#include "csv.h"
#include <QDebug>
Csv::Csv()
{
    file = new QFile;
}

bool Csv::isFileExit(QString id)
{
    file->setFileName(QDir::currentPath()+"//"+id+"_instance.csv");
    return file->open(QIODevice::ReadOnly);
}

void Csv::addRow(QStringList s,QString id)
{
    if(isFileExit(id))
    {
        file->close();
        file->open(QIODevice::Append);
    }
    else
    {
        file->close();
        file->open(QIODevice::WriteOnly);
    }
    QTextStream in(file);
    QString row=in.readAll();
    for(int i=0;i<s.length();i++)
    {
        if(i!=s.length()-1)
            row = row + s.at(i) + ",";
        else
            row = row + s.at(i) + "\n";
    }
    in<<row;
    qDebug()<<"csv called!";
    file->close();
}

QStringList Csv::readAll(QString id)
{
    QStringList buff;
    if(isFileExit(id))
    {
        file->close();
        file->open(QIODevice::ReadOnly);
    }
    else
        return buff;
    QTextStream out(file);
    buff = out.readAll().split("\n");
    return buff;
}
