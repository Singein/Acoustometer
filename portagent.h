#ifndef PORTAGENT_H
#define PORTAGENT_H
#define ORDER_GET_DEVICE_LIST 0
#define ORDER_UPLOAD_HISTORY_DATA 1
#define ORDER_START_COLLECTING 2
#define ORDER_STOP_COLLECTING 3
#define ORDER_CHANGE_SETTINGS 4
#define ORDER_GET_SETTINGS 5
#define ORDER_READ_INSTANCE_DATA 6

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QThread>
#include <datasaver.h>
#include <QMap>
#include <QQueue>
#include <QTimer>

class DataSaver;

class PortAgent:public QObject
{
     Q_OBJECT
public:
    PortAgent();
    ~PortAgent();
    QSerialPort *port;
    DataSaver *DS;
    QMap<QString,int>* map;
    void Set_Settings(QString Settings);//设置下位机即将要应用的参数
    void setPort(QSerialPort *P);
    bool isStarted;
    int  getT();
    void setMap(QMap<QString,int>* Map);
    bool isDataRecived;
    QQueue<QString> orderList;

private:
    int T;
    QThread *thread;
    QTimer *timer;
    QString recivedTime;
    QString settings; //存储下位机要修改的参数设置
    QString Order_Get_Settings(int id);//获取当前下位机参数指令
    QString Order_Change_Settings(int id);//更改下位机参数指令
    QString Order_Get_Device_List(int id);//获取时间组列表数据
    QString Order_Get_Time_Point(int id);//获取某一id的时间点数据
    QString Order_Upload_History_Data(int id,QString time);//获取某一特定编号的时间组数据
    QString Order_Read_Instance_Data(int id);//获取实时数据
    void Order_Start_Read_Instance(int id);//开始采集
    void Order_Stop_Read_Instance(int id);//停止采集
    QStringList Raw_Data_Instance(QByteArray* rec);//实时数据处理方法
    QStringList Raw_Data_TimePoint(QByteArray* rec);//历史数据组列表处理方法
    QStringList Raw_Data_History(QByteArray* rec);//某一编号时间组数据处理方法
    QStringList Raw_Data_Settings(QByteArray* rec);//下位机当前参数数据处理方法
    QString Raw_Data_ID(QByteArray* rec);//下位机ID
    int Raw_Data_FalseMessage(QByteArray* rec);//下位机错误信息反馈处理方法
    void Data_Instance(QByteArray rec);
    void Data_History(QByteArray rec);
    void Data_TimePoint(QByteArray rec);
    void Data_Settings(QByteArray rec);
    void Data_ID(QByteArray rec);
    void Data_FalseMessage(QByteArray rec);
    QString expand(QString unexpand);
    QString modIdExpand(int id);
    QString zero;
    bool ok;

signals:
    taskFinished(int order,QString s);
    addTreeNode(QStringList s);
    fillTable(QStringList s);
    readInstanceData(QStringList data);
    addPlotNode(double x,double y,QString id);
    addDeviceId(QString id); //for init
    writeCsv(QStringList s,QString id);
    deviceParameter(QStringList settings);
    fakeTimer(int order,int id);
    falseMessage(int kind);
    send();
    connectError();

public slots:
    void GiveOrders(int order,int id);
    void GiveOrders(int id,QString time);
    void OrderExcuted();
    void fakeTimerSlot(int order,int id);
    void SendOrders();
    void setT(int t);
    void connectionError();
};

#endif // PORTAGENT_H
