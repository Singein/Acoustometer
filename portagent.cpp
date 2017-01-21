#include "portagent.h"

PortAgent::PortAgent(QSerialPort *Port)
{
    this->port = Port;

}

PortAgent::PortAgent()
{
    OperateDataThread = new QThread;
    WriteDataBaseThread = new QThread;
}

void PortAgent::setPort(QSerialPort *p)
{
    this->port = p;
    moveToThread(OperateDataThread);
    connect(port,SIGNAL(readyRead()),this,SLOT(OrderExcuted()));
    OperateDataThread->start();
}

PortAgent::~PortAgent()
{

}

void PortAgent::GiveOrders(int order,int id)
{
    switch (order) {
    case ORDER_SHOW_COLLECTED_DATA:Order_Show_Collected_Data(id);break;
    case ORDER_UPLOAD_COLLECTED_DATA:Order_Upload_Collected_Data(id);break;
    case ORDER_START_COLLECTING:Order_Start_Collecting(id);break;
    case ORDER_STOP_COLLECTING:Order_Stop_Collecting(id);break;
    case ORDER_CHANGE_SETTINGS:Order_Change_Settings(id,settings);break;
    case ORDER_GET_SETTINGS:Order_Get_Settings(id);break;
    default:break;
    }
}

void PortAgent::OrderExcuted()
{
    QByteArray data = port->readAll();
    //TODO:operation the data

    //第一步取到第一位 机器的ID
    //第二部判断功能码+寄存器，如果功能码是用来读取历史数据时间组的，把解析后的数据以QString格式发送
    //emit addTreeNode(s);

    moveToThread(WriteDataBaseThread);

    //如果是实时数据，往数据库里扔，扔完然后发信号
    //emit readInstanceData();
    //如果是历史数据，直接往数据库里扔

    WriteDataBaseThread->start();

}

void PortAgent::Order_Show_Collected_Data(int id)
{
    QString test = "0103050000000000";
    QByteArray s = QByteArray::fromHex(test.toLatin1().data());
    port->write(s,s.length());
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Upload_Collected_Data(int id)
{
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Start_Collecting(int id)
{
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Stop_Collecting(int id)
{
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Get_Settings(int id)
{
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Change_Settings(int id,QStringList Settings)
{
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Set_Settings(QStringList Settings)
{
    this->settings = Settings;
}
