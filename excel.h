#ifndef EXCEL_H
#define EXCEL_H

#include <csv.h>

class Excel : public QObject
{
    Q_OBJECT
public:
    explicit Excel(QObject *parent = 0);
    ~Excel();
    void Export(QString filename,QStringList data);
    void setCsv(Csv *csv);

signals:
    void current_progress(double progress);

private:
    Csv *csv;

};

#endif // EXCEL_H
