#include "portagent.h"
#include "QDebug"
#include "crccheck.h"

PortAgent::PortAgent(QSerialPort *Port)
{
    this->port = Port;
    DB = new Database;
}

PortAgent::PortAgent()
{
    ReciveDataThread = new QThread;
    OperateDataThread = new QThread;
    DB = new Database;
    qDebug()<<"当前线程ID:"<<QThread::currentThreadId()<<" port agent 已就位";
//    DB->createTable();
}

void PortAgent::setPort(QSerialPort *p)
{
    this->port = p;
    qDebug()<<"port agent 已接管串口";
    qDebug()<<"当前线程ID:"<<QThread::currentThreadId();
    moveToThread(ReciveDataThread);
    qDebug()<<"正在移交至新线程";
    connect(port,SIGNAL(readyRead()),this,SLOT(OrderExcuted()),Qt::QueuedConnection);//消息队列模式
    qDebug()<<"消息队列模式--串口通信已建立";
    ReciveDataThread->start();




}

void PortAgent::Set_Settings(QString Settings)
{
    this->settings = Settings;
}

void PortAgent::setMap(QMap<QString, int> *Map)
{
    this->map = Map;
    qDebug()<<"开关表映射 已建立";
}

PortAgent::~PortAgent()
{

}

void PortAgent::GiveOrders(int order,int id)
{
    QStringList orders;
    switch (order) {
    case ORDER_GET_DEVICE_LIST:orders.append(Order_Get_Device_List(id));break;
    case ORDER_UPLOAD_HISTORY_DATA:orders.append(Order_Upload_History_Data(id));break;
    case ORDER_START_COLLECTING:Order_Start_Read_Instance(id);break;
    case ORDER_STOP_COLLECTING:Order_Stop_Read_Instance(id);break;
    case ORDER_CHANGE_SETTINGS:orders.append(Order_Change_Settings(id));break;
    case ORDER_GET_SETTINGS:orders.append(Order_Get_Settings(id));break;
    case ORDER_READ_INSTANCE_DATA:orders.append(Order_Read_Instance_Data(id));break;
    default:break;
    }

    qDebug()<<"当前线程ID:"<<QThread::currentThreadId()<<" 以下是命令详情，"<<"命令条数："<<orders.length();
    for(int i=0;i<orders.length();i++)
    {
        QString s = orders.at(i);
        QByteArray order = QByteArray::fromHex(s.toLatin1().data());
        port->write(order,order.length());
        qDebug()<<order.toHex().data();
    }
}

void PortAgent::OrderExcuted()
{
//    isDataRecived->stop();
    bool ok;
    qDebug()<<"当前线程ID:"<<QThread::currentThreadId();
    QByteArray data = port->readAll();
    qDebug() << "----volum machine: data recived!"<<" data length: "<<data.length()<<" bytes";
    int len = data.length();//下面就是根据不同的长度，调用不同的数据处理方法 
//    QByteArray noCRCCode = data.mid(0,len-2);
    CrcCheck *crcg = new CrcCheck();
    QByteArray crc = data.mid(len-2,2);
    QString crcCheck = crcg->crcChecksix(data.mid(0,len-2));
//    QByteArray crcCheck = QByteArray::number(qChecksum(QByteArray::fromHex(noCRCCode),len-2)).toUpper();

    int Flag = data.mid(1,1).toInt(&ok,16);
    moveToThread(OperateDataThread);
    OperateDataThread->start();
    if(crc == crcCheck.toLatin1())
    {

        switch(Flag)
        {
            case 3: if(len>8) Data_Settings(data);
                    else Data_Instance(data);break;
            //case 1: Data_History(data);break;
            case 65: Data_TimePoint(data);break;
            case 66: Data_History(data);break;
            //case 3: Data_Settings(data);break;
        default: Error_Data(data);
        }
    }
    //TODO:operation the data
    //第一步取到第一位 机器的ID
    //第二部判断功能码+寄存器，如果功能码是用来读取历史数据时间组的，把解析后的数据以QString格式发送
    //emit addTreeNode(s);
    //如果是实时数据，往数据库里扔，扔完然后发信号
    //emit readInstanceData();
    //如果是历史数据，直接往数据库里扔

}

//--------------------以下是封装的命令-----------------------------
/*16进制字符补足长度*/
QString PortAgent::expand(QString unexpand){
    QString zero = "0";
    QString douZero = "00";
    QString triZero = "000";
    QString expandString;
    switch (unexpand.length()) {
    case 1:expandString = triZero.append(unexpand);break;
    case 2:expandString = douZero.append(unexpand);break;
    case 3:expandString = zero.append(unexpand);break;
    default:
        break;
    }
    return expandString;
}

/*modId字符补足长度*/
QString PortAgent::modIdExpand(int id){
    QString zero = "0";
    QString modId = QString::number(id,16).toUpper();
    if(modId.length()<2){modId = zero.append(modId);}
    return modId;
}

QString PortAgent::Order_Get_Settings(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Get_Settings called";
    //TODO: build the command message
    //TODO: sender->write(message)
    QString readDataRequest;
    CrcCheck *crc = new CrcCheck();
    QString modId = QString::number(id,16);
        QString zero = "0";
    if(modId.length()<2){
    modId = zero.append(modId);
    }
        readDataRequest.append(modId);
        readDataRequest.append("03");    //功能码
        readDataRequest.append("00");    //寄存器地址高位
        readDataRequest.append("00");    //寄存器地址低位
        readDataRequest.append("00");    //寄存器数目高位
        readDataRequest.append("07");    //寄存器数目低位 (这里我分开写清楚点 内啥觉得太长就并成一句好了)
        readDataRequest.append(crc->crcChecksix(readDataRequest));
        return readDataRequest;
}

QString PortAgent::Order_Change_Settings(int id)
{

    qDebug()<<"MODID:"<<id<<" "<<"Order_Change_Settings Gived";
    //TODO: build the command message
    //TODO: sender->write(message)
    qDebug()<<this->settings;
    CrcCheck *crc = new CrcCheck();
    QStringList setting = settings.split(",");
    QString modId = modIdExpand(id);
    QStringList timeSetting = setting[6].split(" ");
    QStringList dateTime = timeSetting[0].split("-");
    QStringList clockTime = timeSetting[1].split(":");
    bool ok;
    for(int it = 0;it<6;++it){
        setting[it] = QString::number(setting[it].toInt(&ok,10),16).toUpper();
        setting[it] = expand(setting[it]);
    }
    for(int it = 0;it<dateTime.length();it++){
        dateTime[it] = QString::number(dateTime[it].toInt(&ok,10),16).toUpper();
        dateTime[it] = expand(dateTime[it]);
    }
    for(int it = 0;it<clockTime.length();it++){
        clockTime[it] = QString::number(clockTime[it].toInt(&ok,10),16).toUpper();
        clockTime[it] = expand(clockTime[it]);
    }
    QString changeDataRequest;
    changeDataRequest.append(modId);
    changeDataRequest.append("10"); //功能码
    changeDataRequest.append("0000");  //寄存器地址
    changeDataRequest.append("0010");  //寄存器数目
    changeDataRequest.append("20");  //字节数
    changeDataRequest.append(setting[0]);  //寄存器[0]位k值
    changeDataRequest.append(setting[2]);   //档位 寄存器
    changeDataRequest.append(setting[5]);  //自动关机时间
    changeDataRequest.append(setting[3]);  //频率关联状态
    changeDataRequest.append(setting[4]); //存储时间间隔
    changeDataRequest.append(setting[1]); //测量量程最大值
    modId = expand(modId);
    changeDataRequest.append(modId);    //485id
    changeDataRequest.append(dateTime[0]);
    changeDataRequest.append(dateTime[1]);
    changeDataRequest.append(dateTime[2]);
    changeDataRequest.append(clockTime[0]);
    changeDataRequest.append(clockTime[1]);
    changeDataRequest.append(clockTime[2]);
    changeDataRequest.append(crc->crcChecksix(changeDataRequest));
    return changeDataRequest;
}

QStringList PortAgent::Order_Get_Device_List(int id)
{
    CrcCheck *crc = new CrcCheck();
    QString radioRequest;
    radioRequest.append(modIdExpand(id)).append("11");
    return radioRequest + crc->crcChecksix(radioRequest);
}

QString PortAgent::Order_Get_Time_Point(int id){
    QString readTimePointRequest;
    CrcCheck *crc = new CrcCheck();
    readTimePointRequest.append(modIdExpand(id)).append("41");
    readTimePointRequest.append(crc->crcChecksix(readTimePointRequest));
    return readTimePointRequest;
}

QString PortAgent::Order_Upload_History_Data(int id)//170301120000
{

    qDebug()<<"MODID:"<<id<<" "<<"Order_Upload_Selected_Data Gived";
    //TODO: build the command message
    //TODO: sender->write(message)
    QString UploadRequest;
    //这里我把 编号 groupid和modId全部合并到id参数中
    //具体做法是，传来的id = modId*10000+groupId*100+num (num代表有多少个数据）
    UploadRequest.append(modIdExpand(id)).append("42"); //添加modid与功能码42
    for(int i = 0;i < timeId.length()/2;i++){
        UploadRequest.append(expand(QString::number(timeId.mid(2*i,2).toInt(),16).toUpper()));
    }
}

QString PortAgent::Order_Read_Instance_Data(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Read_Instance_Data Gived";
    QString readDataRequest;
    CrcCheck *crc = new CrcCheck();
    readDataRequest.append(modIdExpand(id));
    readDataRequest.append("03");    //功能码
    readDataRequest.append("00");    //寄存器地址高位
    readDataRequest.append("0E");    //寄存器地址低位
    readDataRequest.append("00");    //寄存器数目高位
    readDataRequest.append("02");    //寄存器数目低位 (这里我分开写清楚点 内啥觉得太长就并成一句好了)
    readDataRequest.append(crc->crcChecksix(readDataRequest));
    return readDataRequest;
}

void PortAgent::Order_Start_Read_Instance(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Start_Collecting Gived";

    GiveOrders(ORDER_READ_INSTANCE_DATA,id);
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Stop_Read_Instance(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Stop_Collecting Gived";
    //TODO: build the command message
    //TODO: sender->write(message)
}
//---------------------------------------------------------------

//---------------------以下是数据粗加工----------------------------
QStringList PortAgent::Raw_Data_Instance(QByteArray* rec)
{
    QByteArray noCRCdata = rec->mid(0,rec->length()-2);
    QStringList instanceData;
    int modid = (noCRCdata.mid(0,1).toInt(&ok,16));
    bool ok;
//        int num = (noCRCdata.mid(2,1).toInt(&ok,16));
//        int year = (noCRCdata.mid(3,2).toInt(&ok,16));
//        int month = (noCRCdata.mid(5,2).toInt(&ok,16));
//        int day = (noCRCdata.mid(7,2).toInt(&ok,16));
//        int hour = (noCRCdata.mid(9,2).toInt(&ok,16));
//        int minute = (noCRCdata.mid(11,2).toInt(&ok,16));
//        int second = (noCRCdata.mid(13,2).toInt(&ok,16));
    int vol = (noCRCdata.mid(3,2).toInt(&ok,16));
    int fre = (noCRCdata.mid(5,2).toInt(&ok,16));
        //QString s = QString::number(year,10);
        //emit operationFinished(QByteArray::number(vol), QByteArray::number(fre));
    instanceData<<QString::number(modid,10)<<QString::number(vol,10)<<QString::number(fre,10);
    return instanceData;
}

QStringList PortAgent::Raw_Data_TimePoint(QByteArray* rec)
{
    QByteArray noCRCData = rec->mid(0,rec->length()-2);
    QStringList timeTable;
    bool ok;
    int modid = noCRCData.mid(0,1).toInt(&ok,16);
    int fun = noCRCData.mid(1,1).toInt(&ok,16);
    int len = (rec->length()-4)/4;
    QString timeData;
    if (fun == 65)
    {
        for(int i = 0;i < len;i++){
            timeData = QString("%1-%2-%3 %4:%5:%6 %7").arg(QString::number(noCrcData.mid(2+7*i,1).toInt(&ok,16),10)).arg(QString::number(noCrcData.mid(3+7*i,1).toInt(&ok,16),10)).arg(QString::number(noCrcData.mid(4+7*i,1).toInt(&ok,16),10)).arg(QString::number(noCrcData.mid(5+7*i,1).toInt(&ok,16),10)).arg(QString::number(noCrcData.mid(6+7*i,1).toInt(&ok,16),10)).arg(QString::number(noCrcData.mid(7+7*i,1).toInt(&ok,16),10)).arg(QString::number(noCrcData.mid(8+7*i,1).toInt(&ok,16),10));
            timeTable.append(timeData);
        }
//        int num = noCRCData.mid(2,1).toInt(&ok,16);
//        int year = noCRCData.mid(3,1).toInt(&ok,16);
//        int month = noCRCData.mid(4,1).toInt(&ok,16);
//        int day = noCRCData.mid(5,1).toInt(&ok,16);
//        int hour = noCRCData.mid(6,1).toInt(&ok,16);
//        int minute = noCRCData.mid(7,1).toInt(&ok,16);
//        int second = noCRCData.mid(8,1).toInt(&ok,16);
//        int timeGroupNum = noCRCData.mid(9,2).toInt(&ok,16);
//        int startPosition = noCRCData.mid(11,2).toInt(&ok,16);
//        int timeInterval = noCRCData.mid(13,2).toInt(&ok,16);
//        QString time = QString::number(year,10) +"-"+ QString::number(month,10)+"-"+ QString::number(day,10)
//               + " " + QString::number(hour,10) +":" + QString::number(minute,10) +":" + QString::number(second,10);
//        //格式 2017/1/25 17：03：54
//        CollectedData<<QString::number(modid,10)<<time<<QString::number(timeGroupNum,10)
//                 <<QString::number(startPosition,10)<<QString::number(timeInterval,10);
     }   
    DB->createDataTable(QString::number(modid,10)+"_history");
    DB->createDataTable(QString::number(modid,10)+"_instance");
    return timeTable;
}

QStringList PortAgent::Raw_Data_History(QByteArray* rec)
{
    QByteArray noCRCCode = rec->mid(0,7);
    QStringList timePointTable;
    bool ok;
    int modId = noCRCCode.mid(0,1).toInt(&ok,16);
    int fun = noCRCCode.mid(1,1).toInt(&ok,16);
    int len = noCRCCode.mid(10,1).toInt(&ok,16);
    int year = noCRCCode.mid(2,1).toInt(&ok,16);
    int month = noCRCCode.mid(3,1).toInt(&ok,16);
    int day = noCRCCode.mid(4,1).toInt(&ok,16);
    int hour = noCRCCode.mid(5,1).toInt(&ok,16);
    int minute = noCRCCode.mid(6,1).toInt(&ok,16);
    int second = noCRCCode.mid(7,1).toInt(&ok,16);
    for(int i=0;i<len;i++){
        QString timePoint = QString("%1-%2-%3 %4:%5:%6 %7 %8").arg(QString::number(year,10)).arg(QString::number(month,10)).arg(QString::number(day,10)).arg(QString::number(hour,10)).arg(QString::number(minute,10)).arg(QString::number(second,10)).arg(QString::number(noCRCCode.mid(10+len*2,1).toInt(&ok,16)*100,10)).arg(QString::number(noCRCCode.mid(11+len*2,1).toInt(&ok,16)*100,10));
        timePointTable<<timePoint;
        second+=3;
        if(second>=60){second-=60;minute+=1;
        }if(minute>=60){minute-=60;hour+=1;
        }if(hour>=24){hour-=24;day+=1;
        }if(month==1||month==3||month==5||month==7||month==8||month==10||month==12){
            if(day>31){day-=31;month+=1;}
        }if(month==4||month==6||month==9||month==11){
            if(day>30){day-=30;month+=1;}
        }if(month==2 && year%4==0){
            if(day>29){day-=29;month+=1;}
        }if(month==2 && year%4!=0){
            if(day>28){day-=28;month+=1;}
        }if(month>12){month-=1;year+=1;}
    }
    return timePointTable;
}

QStringList PortAgent::Raw_Data_Settings(QByteArray* rec)
{
    QByteArray noCRCdata = rec->mid(0,rec->length()-2);
    QStringList settings;
    int modid = (noCRCdata.mid(0,1).toInt(&ok,16));
    int fun = noCRCdata.mid(1,1).toInt(&ok,16);
    bool ok;
    int num = (noCRCdata.mid(2,1).toInt(&ok,16));
//    for(int i = 0;i < num;i++){settings<<QString::number(noCRCdata.mid(3+2*i,2).toInt(&ok,16));}
    int kValue = (noCRCdata.mid(3,2).toInt(&ok,16));
    int rangeMax = (noCRCdata.mid(5,2).toInt(&ok,16))*100;
    int SaveGearsValue = (noCRCdata.mid(7,2).toInt(&ok,16));
    int connectFrqcyOnOff = (noCRCdata.mid(9,2).toInt(&ok,16));
    int saveTimeInterval = (noCRCdata.mid(11,2).toInt(&ok,16));
        //QString s = QString::number(year,10);
        //emit operationFinished(QByteArray::number(vol), QByteArray::number(fre));
    settings<<QString::number(kValue,10)<<QString::number(rangeMax,10)<<QString::number(SaveGearsValue,10)<<QString::number(connectFrqcyOnOff,10)<<QString::number(saveTimeInterval,10);
    return settings;
}
//----------------------------------------------------------------

//--------------------以下是数据精加工------------------------------
void PortAgent::Data_Instance(QByteArray data)
{
    QStringList dataList;
    dataList = Raw_Data_Instance(&data);
    if(map->value(dataList.at(0)) == 1)  //判断实时数据的采集开关状态
    {
        QDateTime time = QDateTime::currentDateTime();//为了保证写入数据库的和界面实时更新的时间一致
        dataList<<time.toString("yyyy-MM-dd hh:mm:ss");
        emit readInstanceData(dataList);//把存入数据库的时间同时发给主界面
        DB->insertInstanceDataTable(dataList.at(0)+"_instance",time,dataList.at(1),dataList.at(2));
    }
}

void PortAgent::Data_History(QByteArray data)
{
    Raw_Data_History(&data);

    //DB->insertHistoryDataTable();
}

void PortAgent::Data_TimePoint(QByteArray data)
{
    DB->insertTimeGroupTable(Raw_Data_TimePoint(&data));
}

void PortAgent::Data_Settings(QByteArray data)
{
    emit deviceParameter(Raw_Data_Settings(&data));
}
//----------------------------------------------------------------

//--------------------以下是错误处理------------------------------
QString PortAgent::Error_Data(QByteArray* rec){
    int id = rec->mid(0,1).toInt(ok,16);
    int fun = rec->mid(1,1).toInt(ok,16) - 16*8;
    int flag = rec->mid(2,1).toInt(ok,16);
    switch (flag){
        case 1:return "错误功能码";break;
        case 2:return "错误地址";break;
        case 3:return "错误数据";break;
        case 6:return "从设备忙";break;
}
}
