#ifndef CSV_H
#define CSV_H

#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QTextStream>

#define PATH "/instance_buff"

class Csv
{
public:
    Csv();
    void addRow(QStringList s,QString id);
    QStringList readAll(QString id);

private:
    QFile *file;
    bool isFileExit(QString id);

};

#endif // CSV_H
