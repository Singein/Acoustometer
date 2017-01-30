#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDateTime>

class Database
{
public:
    Database();
    bool createConnection();
    void createDataTable(QString tableName);
    void createTimeGroupTable();
    void insertInstanceDataTable(QString tableName,QString frequency,QString soundStrength);
    void insertHistoryDataTable(QString tableName,QString time,QString frequency,QString soundStrength);
    bool insertTimeGroupTable(QStringList timeGroups);
    void updataTable(QString tableName,QString columnName,QString time,QString change);
    void deleteTable(QString tableName,QString columnName,QString time);
    QStringList queryDataTableAll(QString tableName);
    QString queryTable(QString tableName,QString time);
    QStringList getTimePoint(QString tableName);
    QStringList getTimePointInfo(QString tableName);
    QString queryDataTable(QString tableName,QString time);
    QStringList getSelectedData(QString tableName,QStringList timePoints);
//    void queryTableTimeCorp(QString tableName,QString startTime,QString endTime);
    void dropTable(QString tableName);
    ~Database();
private:
    QSqlDatabase db;

};

#endif // DATABASE_H
