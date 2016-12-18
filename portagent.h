#ifndef PORTAGENT_H
#define PORTAGENT_H
#define ORDER_SHOW_COLLECTED_DATA 0
#define ORDER_UPLOAD_COLLECTED_DATA 1
#define ORDER_START_COLLECTING 2
#define ORDER_STOP_COLLECTING 3
#define ORDER_CHANGE_SETTINGS 4
#define ORDER_GET_SETTINGS 5

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QThread>

class PortAgent:public QObject
{
     Q_OBJECT
public:
    PortAgent(QSerialPort *Port);
    PortAgent();
    ~PortAgent();
    void GiveOrders(int order,int id);
    void Set_Settings(QStringList Settings);
    QSerialPort *port;
    QThread *OperateDataThread;
    QThread *WriteDataBaseThread;
    void setPort(QSerialPort *P);

private:
    QStringList settings;
    void Order_Get_Settings(int id);
    void Order_Change_Settings(int id,QStringList Settings);//更改设置
    void Order_Start_Collecting(int id);//开始采集
    void Order_Stop_Collecting(int id);//停止采集
    void Order_Show_Collected_Data(int id);//依照时间分组的采集数据
    void Order_Upload_Collected_Data(int id);//上传历史数据

signals:
    taskFinished(int order,QString s);
    addTreeNode(QString s);
    readInstanceData();

public slots:
    void OrderExcuted();
};

#endif // PORTAGENT_H
