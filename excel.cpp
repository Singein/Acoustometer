#include "excel.h"

Excel::Excel(QObject *parent) : QObject(parent)
{

}

Excel::~Excel()
{

}

void Excel::setCsv(Csv *csv)
{
    this->csv = csv;
}

void Excel::Export(QString filename, QStringList data)
{
    if (filename.isEmpty()) {
        return;
    }
    QStringList header;
    header<<"时间"<<"声强"<<"频率";
    for(int i=0;i<=data.length();i++)
    {
        if(i==0)
            csv->addRow(header,filename);
        else
            csv->addRow(data.at(i-1).split(","),filename);
        emit current_progress(((double)i)/((double)data.length()));
    }
}


