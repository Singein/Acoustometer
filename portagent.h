#ifndef PORTAGENT_H
#define PORTAGENT_H

#define ORDER_SHOW_COLLECTED_DATA 0
#define ORDER_UPLOAD_SELECTED_DATA 1
#define ORDER_START_COLLECTING 2
#define ORDER_STOP_COLLECTING 3
#define ORDER_CHANGE_SETTINGS 4
#define ORDER_GET_SETTINGS 5
#define ORDER_READ_INSTANCE_DATA 6

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QThread>
#include <database.h>
#include <QQueue>

class PortAgent:public QObject
{
     Q_OBJECT
public:
    PortAgent(QSerialPort *Port);
    PortAgent();
    ~PortAgent();
    void GiveOrders(int order,int id);
    void Set_Settings(QString Settings);//设置下位机即将要应用的参数
    QSerialPort *port;
    QThread *ReciveDataThread;
    QThread *OperateDataThread;
    QQueue<QString> timequeue;
    Database *DB;
    void setPort(QSerialPort *P);
    bool isStarted;

private:
    QString settings; //存储下位机要修改的参数设置
    QString Order_Get_Settings(int id);//获取当前下位机参数指令
    QString Order_Change_Settings(int id);//更改下位机参数指令
    QStringList Order_Show_Collected_Data(int id);//获取时间组列表数据
    QStringList Order_Upload_Selected_Data(int id);//获取某一特定编号的时间组数据
    QString Order_Read_Instance_Data(int id);//获取实时数据
    void Order_Start_Read_Instance(int id);//开始采集
    void Order_Stop_Read_Instance(int id);//停止采集

    QStringList Data_Instance(QByteArray rec);//实时数据处理方法
    QStringList Data_Collected(QByteArray rec);//历史数据组列表处理方法
    QStringList Data_Selected(QByteArray rec);//某一编号时间组数据处理方法
    QStringList Data_Settings(QByteArray rec);//下位机当前参数数据处理方法

    QString zero;
    bool ok;

signals:
    taskFinished(int order,QString s);
    addTreeNode(QStringList s);
    readInstanceData(QStringList data);

public slots:
    void OrderExcuted();
};

#endif // PORTAGENT_H
