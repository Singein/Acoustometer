#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlTableModel>
#include <QDateTime>
#include "qstringlist.h"


Database::Database()
{
    createConnection();
    createTimeGroupTable();
}

/*创建链接，建立数据表*/
bool Database::createConnection(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("strengh.db");
    if(!db.open()){
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
        return false;
    }
    qDebug()<<"Data Base 已连接";
    return true;
}

bool Database::isTableExist(QString tableName)
{
    db.open();
    QSqlQuery query(db);
    return query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(tableName));    //判断表是否存在
}

/*建立实时数据表*/
void Database::createDataTable(QString tableName){
    db.open();
    QSqlQuery query(db);
    bool TableExist = isTableExist(tableName);    //判断表是否存在
    if(!db.open()){
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
    }
    if(!TableExist)
        query.exec(QString("create table "+tableName+" (time varchar(30) primary key,frequency varchar(5),soundStrength varchar(3))")); //实时数据与历史数据

}

/*建立时间组数据表*/
void Database::createTimeGroupTable(){
    db.open();
    QSqlQuery query(db);
    if(!db.open()){
        QMessageBox::critical(0,"cannot open database","Unable to establish a database connect",QMessageBox::Cancel);
    }
    query.exec(QString("create table timeGroup (id integer primary key autoincrement,modId int,timePoint varchar(30),groups int,interval varchar(5))")); //时间组表，主键为id
    qDebug()<<"TimeGroupTable 已建立";
}

/*插入实时数据*/
void Database::insertInstanceDataTable(QString tableName,QDateTime currentTime,QString frequency,QString soundStrength){
    db.open();
    QSqlQuery query(db);
    QString time = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    if(db.open()){
        query.exec(QString("insert into "+tableName+" values('"+time+"','"+frequency+"','"+soundStrength+"')"));
        qDebug()<<"实时数据已插入 "<<tableName;
    }else{
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
    }
}

void Database::insertHistoryDataTable(QString tableName, QString time, QString frequency, QString soundStrength)
{
    db.open();
    QSqlQuery query(db);
    if(db.open()){
        query.exec(QString("insert into "+tableName+" values('"+time+"','"+frequency+"','"+soundStrength+"')"));
    }else{
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
    }
}

/*插入时间组数据*/
bool Database::insertTimeGroupTable(QStringList timeGroups){
    bool ok;
    db.open();
    QSqlQuery query(db);
    if(db.open()){
        query.exec(QString("insert into timeGroup values('null','"+timeGroups.at(0)+"','"+timeGroups.at(1)+"','"+timeGroups.at(2)+"','"+timeGroups.at(3)+"')"));
        ok = true;  //插入成功，返回true
    }else{
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
        ok = false; //插入失败，返回false
    }
    qDebug()<<"timegrop table insert ok";
    return ok;
}

/*更改数据,没用的话可以删掉*/
void Database::updataTable(QString tableName,QString columnName,QString time,QString change){
    QSqlQuery query(db);
    db.open();
    query.exec(QString("update table "+tableName+" SET "+columnName+" = "+change+" WHERE time = '"+time+"'"));
}

/*删除实时数据表某一行数据*/
void Database::deleteTable(QString tableName,QString columnName,QString time){
    db.open();
    QSqlQuery query(db);
    query.exec(QString("delete from "+tableName+" where "+columnName+" = '"+time+"'"));
}

/*查询实时数据表中所有数据*/
QStringList Database::queryDataTableAll(QString tableName){
    db.open();
    QSqlQuery query(db);
    QString data0;
    QString data1;
    QString data2;
    query.exec(QString("select * from "+tableName));
    QStringList list;
    while(query.next()){
        data0 = query.value(0).toString();
        data1 = query.value(1).toString();
        data2 = query.value(2).toString();
        data0.append(" ");
        data0.append(data1);
        data0.append(" ");
        data0.append(data2);
        list.append(data0);
    }
    qDebug()<<"queryDataTableAll called!";

    return list;
}

/*查询时间组表中时间点*/
QStringList Database::getTimePoint(QString tableName){
    db.open();
    QSqlQuery query(db);
    QString data;
    QString data0 = "";
    QString data1;
    query.exec(QString("select * from "+tableName));
    QStringList list;
    while(query.next()){
        if(data0 != query.value(0)){
            data0 = query.value(0).toString();
            data1 = query.value(1).toString();
            data.append(data0);
            data.append(",");
            data.append(data1);
        }else{
            data1 = query.value(1).toString();
            data.append(data1);
        }

//        data2 = query.value(2).toString();
//        data3 = query.value(3).toString();
//        data0.append(",");

        list<<data;
    }
    return list;
}

/*查询时间组表中频率与声强*/
QStringList Database::getTimePointInfo(QString tableName){
    db.open();
    QSqlQuery query(db);
    QString data0;
    QString data1;
    QString data2;
    QString data3;
    query.exec(QString("select * from "+tableName));
    QStringList list;
    while(query.next()){
        data0 = query.value(0).toString();
        data1 = query.value(1).toString();
        data2 = query.value(2).toString();
        data3 = query.value(3).toString();
        data2.append(",");
        data2.append(data3);
        list<<data2;
    }
    return list;
}

/*查询实时时间表中某一条记录*/
QString Database::queryDataTable(QString tableName,QString time){
    db.open();
    QSqlQuery query(db);
    QString data0;
    QString data1;
    QString data2;
    query.exec(QString("select * from "+tableName+" where time = "+time));
    while (query.next()){
        data0 = query.value(0).toString();
        data1 = query.value(1).toString();
        data2 = query.value(2).toString();
        data0.append(" ");
        data0.append(data1);
        data0.append(" ");
        data0.append(data2);
        qDebug()<<data0;
    }
    return data0;
}

/*查询实时时间表中某段时间内数据*/
QStringList Database::getSelectedData(QString tableName,QStringList timePoints){
    db.open();
    QSqlQuery query(db);
    QStringList selectedData;
    QString data0;
    QString data1;
    QString data2;
    query.exec(QString("select * from "+tableName+" where timePoint between "+timePoints.at(0)+" and "+timePoints.at(1)));
    while (query.next()){
        data0.append(" ");
        data0.append(data1);
        data0.append(" ");
        data0.append(data2);
        selectedData<<data0;
    }
    return selectedData;
}

/*删表跑路*/
void Database::dropTable(QString tableName){
    db.open();
    QSqlQuery query(db);
    query.exec(QString("drop table "+tableName));
}

Database::~Database()
{

}

