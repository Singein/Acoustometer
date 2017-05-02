#include "portagent.h"
#include "QDebug"
#include "crccheck.h"
#include "QMap"
#include <QString>

PortAgent::PortAgent()
{
//    DB = new Database;
    DS = new DataSaver;
    T = 1000;
    isDataRecived = true;
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
}

void PortAgent::setPort(QSerialPort *p)
{
    port = p;
    connect(port,SIGNAL(readyRead()),this,SLOT(OrderExcuted()));
    connect(this,SIGNAL(fakeTimer(int,int)),this,SLOT(fakeTimerSlot(int,int)));
    connect(this,SIGNAL(send()),this,SLOT(SendOrders()));
    connect(this,SIGNAL(writeCsv(QStringList,QString)),DS,SLOT(writeCsv(QStringList,QString)));

}

void PortAgent::Set_Settings(QString Settings)
{
    this->settings = Settings;
}

void PortAgent::Set_timeId(QString TimeId)
{
    QThread::msleep(1);
    this->timeId = TimeId;
}

void PortAgent::setT(int t)
{
    this->T = t;
}

int PortAgent::getT()
{
    return this->T;
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
    switch (order) {
    case ORDER_GET_DEVICE_LIST:orderList.enqueue(Order_Get_Device_List(id));break;
    case ORDER_UPLOAD_HISTORY_DATA:orderList.enqueue(Order_Upload_History_Data(id));break;
    case ORDER_START_COLLECTING:Order_Start_Read_Instance(id);break;
    case ORDER_STOP_COLLECTING:Order_Stop_Read_Instance(id);break;
    case ORDER_CHANGE_SETTINGS:orderList.enqueue(Order_Change_Settings(id));break;
    case ORDER_GET_SETTINGS:orderList.enqueue(Order_Get_Settings(id));break;
    case ORDER_READ_INSTANCE_DATA:orderList.enqueue(Order_Read_Instance_Data(id));break;
    default:break;
    }

//    if(orderList.length()==rowcount)
//    {
//        qDebug()<<orderList;
        emit send();
//        rowcount = 1;
//    }
}

void PortAgent::SendOrders()
{
    if(isDataRecived)
        if(!orderList.isEmpty())
        {
            QString s = orderList.dequeue();
            QByteArray order = QByteArray::fromHex(s.toLatin1().data());
            port->write(order,order.length());
            isDataRecived = false;
        }
}

void PortAgent::OrderExcuted()
{
    QThread::msleep(1);
    QDateTime time = QDateTime::currentDateTime();//为了保证写入数据库的和界面实时更新的时间一致
    recivedTime = time.toString("yyyy-MM-dd hh:mm:ss");
    bool ok;
    QByteArray data = port->readAll();
//    qDebug() << "data recived ---> "<<"data length: "<<data.length()<<" bytes";
//    qDebug() <<data.toHex();
    int len = data.length();    //下面就是根据不同的长度，调用不同的数据处理方法
    CrcCheck *crcg = new CrcCheck();
    QString crc = data.mid(len-2,2).toHex().toUpper();
    QString crcCheck = crcg->crcChecksix(data.mid(0,len-2).toHex());

    int Flag = data.mid(1,1).toHex().toInt(&ok,16);
    if(crc!=crcCheck){
        switch (Flag) {
        case 3:
            while(len<9){
                data.append(port->readAll());
                len = data.length();
            }
            break;
        case 65:
            while((len-4)%7!=0){
                data.append(port->readAll());
                len = data.length();
            }
            break;
        case 66:
            while((len-12)%4!=0){
                data.append(port->readAll());
                len = data.length();
            }
            break;
        default:
            break;
        }

        crc = data.mid(len-2,2).toHex().toUpper();
        crcCheck = crcg->crcChecksix(data.mid(0,len-2).toHex());
    }
    qDebug()<<"data recived --> "<<data.toHex()<<"\ndata length: "<<data.length()<<" bytes";
//    if(crc == crcCheck)
//    {
        switch(Flag)
        {
            case 2: Data_ID(data);
            case 3: if(len>9)
                        Data_Settings(data);
                    else
                        Data_Instance(data);break;
            case 65:Data_TimePoint(data);break;
            case 66: Data_History(data);break;

        }
//    }
    if(data.length()!=0)
        isDataRecived = true;
    emit send();
}

//TODO:实时数据采集的时间间隔
void PortAgent::fakeTimerSlot(int order,int id)
{
    for (QMap<QString, int>::const_iterator it = map->cbegin(), end = map->cend(); it != end; ++it) {
        if(it.value()==1)
        {
            QThread::msleep(T);
            GiveOrders(order,id);
        }
     }
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
    case 4:return unexpand;
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
    QString readDataRequest;
    CrcCheck *crc = new CrcCheck();
    QString modId = modIdExpand(id);
    readDataRequest.append(modId);
    readDataRequest.append("03");    //功能码
    readDataRequest.append("00");    //寄存器地址高位
    readDataRequest.append("00");    //寄存器地址低位
    readDataRequest.append("00");    //寄存器数目高位
    readDataRequest.append("06");    //寄存器数目低位 (这里我分开写清楚点 内啥觉得太长就并成一句好了)
    readDataRequest.append(expand(crc->crcChecksix(readDataRequest)));
    qDebug()<<"test:"<<readDataRequest<<" crc:"<<expand(crc->crcChecksix(readDataRequest));
    return readDataRequest;
}

QString PortAgent::Order_Change_Settings(int id)
{
    qDebug()<<this->settings;
    CrcCheck *crc = new CrcCheck();
    QStringList setting = settings.split(",");
    QString modId = modIdExpand(id);
    QStringList timeSetting = setting[6].split(" ");
    QStringList dateTime = timeSetting[0].split("-");
    QStringList clockTime = timeSetting[1].split(":");
    QString rangeMax = QString::number((int)(setting[1].toDouble()*100),16).toUpper();
    bool ok;
    for(int it = 0;it<4;++it){
        setting[it] = QString::number(setting[it].toInt(&ok,10),16).toUpper();
        setting[it] = expand(setting[it]);
    }
    for(int it = 4;it<6;it++){
        if(setting.at(it).length()>1){
            setting[it] = QString::number(setting[it].split("#").at(1).toInt(&ok,10),16).toUpper();
            setting[it] = expand(setting[it]);
        }
        else{
            setting[it] = QString::number(setting[it].toInt(&ok,10),16).toUpper();
            setting[it] = expand(setting[it]);
        }
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
    changeDataRequest.append("000E");  //寄存器数目
    changeDataRequest.append("1C");  //字节数
    changeDataRequest.append(setting[0]);  //寄存器[0]位k值
    changeDataRequest.append(expand(rangeMax));   //量程最大值
    changeDataRequest.append(setting[2]);  //档位
    changeDataRequest.append(setting[3]);  //频率关联状态
    changeDataRequest.append(setting[4]); //存储时间间隔
    changeDataRequest.append(setting[5]); //自动关机时间
    modId = expand(modId);
    changeDataRequest.append(modId);    //485id
    changeDataRequest.append("0000");
    changeDataRequest.append(dateTime[0]);
    changeDataRequest.append(dateTime[1]);
    changeDataRequest.append(dateTime[2]);
    changeDataRequest.append(clockTime[0]);
    changeDataRequest.append(clockTime[1]);
    changeDataRequest.append(clockTime[2]);
    changeDataRequest.append(crc->crcChecksix(changeDataRequest));
    return changeDataRequest;
}

QString PortAgent::Order_Get_Device_List(int id)
{
    CrcCheck *crc = new CrcCheck();
    QString radioRequest;
    radioRequest.append(modIdExpand(id)).append("41");
    radioRequest.append(crc->crcChecksix(radioRequest));
    return radioRequest;
}

QString PortAgent::Order_Get_Time_Point(int id){
    QString readTimePointRequest;
    CrcCheck *crc = new CrcCheck();
    readTimePointRequest.append(modIdExpand(id)).append("41");
    readTimePointRequest.append(crc->crcChecksix(readTimePointRequest));
    return readTimePointRequest;
}

QString PortAgent::Order_Upload_History_Data(int id)
{
    CrcCheck *crc = new CrcCheck();
    QString UploadRequest;
    UploadRequest.append(modIdExpand(id)).append("42"); //添加modid与功能码42
    qDebug()<<timeId;
    QStringList timeSettings = this->timeId.split(" ");
    QStringList dateSettings = timeSettings.at(0).split("-");
    QStringList clockSettings = timeSettings.at(1).split(":");
    UploadRequest.append(modIdExpand(dateSettings.at(0).toInt(&ok,10)))
            .append(modIdExpand(dateSettings.at(1).toInt(&ok,10)))
            .append(modIdExpand(dateSettings.at(2).toInt(&ok,10)))
            .append(modIdExpand(clockSettings.at(0).toInt(&ok,10)))
            .append(modIdExpand(clockSettings.at(1).toInt(&ok,10)))
            .append(modIdExpand(clockSettings.at(2).toInt(&ok,10)));
    UploadRequest.append(crc->crcChecksix(UploadRequest));
    qDebug()<<UploadRequest;
    return UploadRequest;
}

QString PortAgent::Order_Read_Instance_Data(int id)
{
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
//    GiveOrders(ORDER_READ_INSTANCE_DATA,id);
    emit fakeTimer(ORDER_READ_INSTANCE_DATA,id);

}

void PortAgent::Order_Stop_Read_Instance(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Stop_Collecting Gived";
}

QStringList PortAgent::Raw_Data_Instance(QByteArray* rec)
{
    QByteArray noCRCdata = rec->mid(0,rec->length()-2);
    QStringList instanceData;
    int modid = (noCRCdata.mid(0,1).toHex().toInt(&ok,16));
    bool ok;
    double vol = (double)((noCRCdata.mid(3,2).toHex().toInt(&ok,16)))/100;
    double fre = (double)(noCRCdata.mid(5,2).toHex().toInt(&ok,16))/100;
    instanceData<<QString::number(vol)<<QString::number(fre)<<QString::number(modid);
    return instanceData;
}

QStringList PortAgent::Raw_Data_TimePoint(QByteArray* rec)
{
    QByteArray noCRCData = rec->mid(0,rec->length()-2);
    QStringList timeTable;
    bool ok;
    int modid = noCRCData.mid(0,1).toHex().toInt(&ok,16);
    int len = (rec->length()-4)/7;
    QString timeData;
    timeTable.append(QString::number(modid));
    for(int i = 0;i < len;i++){
        timeData = QString("%1-%2-%3 %4:%5:%6 %7")
                .arg(QString::number(noCRCData.mid(2+7*i,1).toHex().toInt(&ok,16),10))
                .arg(QString::number(noCRCData.mid(3+7*i,1).toHex().toInt(&ok,16),10))
                .arg(QString::number(noCRCData.mid(4+7*i,1).toHex().toInt(&ok,16),10))
                .arg(QString::number(noCRCData.mid(5+7*i,1).toHex().toInt(&ok,16),10))
                .arg(QString::number(noCRCData.mid(6+7*i,1).toHex().toInt(&ok,16),10))
                .arg(QString::number(noCRCData.mid(7+7*i,1).toHex().toInt(&ok,16),10))
                .arg(QString::number(noCRCData.mid(8+7*i,1).toHex().toInt(&ok,16),10));
        timeTable.append(timeData);
    }
    qDebug()<<timeTable;
    return timeTable;
}

QStringList PortAgent::Raw_Data_History(QByteArray* rec)
{
    QByteArray noCRCCode = rec->mid(0,rec->length()-2);
    QStringList timePointTable;
    bool ok;
    int modId = noCRCCode.mid(0,1).toHex().toInt(&ok,16);
//    int fun = noCRCCode.mid(1,1).toHex().toInt(&ok,16);
    int year = noCRCCode.mid(2,1).toHex().toInt(&ok,16);
    int month = noCRCCode.mid(3,1).toHex().toInt(&ok,16);
    int day = noCRCCode.mid(4,1).toHex().toInt(&ok,16);
    int hour = noCRCCode.mid(5,1).toHex().toInt(&ok,16);
    int minute = noCRCCode.mid(6,1).toHex().toInt(&ok,16);
    int second = noCRCCode.mid(7,1).toHex().toInt(&ok,16);
    int timeInterval = noCRCCode.mid(8,1).toHex().toInt(&ok,16);
    int len = noCRCCode.mid(9,1).toHex().toInt(&ok,16);
    timePointTable<<QString::number(modId,10)<<QString::number(len,10);

    QString y = QString::number(year,10);
    QString m = QString::number(month,10);
    QString d = QString::number(day,10);
    QString h = QString::number(hour,10);
    QString M = QString::number(minute,10);
    QString s = QString::number(second,10);

    if(y.length()<4)
        y = "20"+y;
    if(m.length()<2)
        m = "0"+m;
    if(d.length()<2)
        d = "0"+d;
    if(h.length()<2)
        h = "0"+h;
    if(M.length()<2)
        M = "0"+M;
    if(s.length()<2)
        s = "0"+s;

    for(int i=0;i<len;i++){
        QString timePoint = QString("%1 %2 %3-%4-%5 %6:%7:%8").arg(QString::number(((double)noCRCCode.mid(10+i*4,2).toHex().toInt(&ok,16))/100)).arg(QString::number(((double)noCRCCode.mid(12+i*4,2).toHex().toInt(&ok,16))/100)).arg(y).arg(m).arg(d).arg(h).arg(M).arg(s);
        timePointTable<<timePoint;
        second+=timeInterval;
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
    qDebug()<<timePointTable;
    return timePointTable;
}

QStringList PortAgent::Raw_Data_Settings(QByteArray* rec)
{
    QByteArray noCRCdata = rec->mid(0,rec->length()-2);
    QStringList settings;
//    int modid = (noCRCdata.mid(0,1).toHex().toInt(&ok,16));
//    int fun = noCRCdata.mid(1,1).toHex().toInt(&ok,16);
//    bool ok;
//    int num = (noCRCdata.mid(2,1).toHex().toInt(&ok,16));
//    for(int i = 0;i < num;i++){settings<<QString::number(noCRCdata.mid(3+2*i,2).toInt(&ok,16));}
    int kValue = (noCRCdata.mid(3,2).toHex().toInt(&ok,16));
    double rangeMax = (double)(noCRCdata.mid(5,2).toHex().toInt(&ok,16))/100;
    int connectFrqcyOnOff = (noCRCdata.mid(7,2).toHex().toInt(&ok,16));
    int SaveGearsValue = (noCRCdata.mid(9,2).toHex().toInt(&ok,16));
    int saveTimeInterval = (noCRCdata.mid(11,2).toHex().toInt(&ok,16));
    int autoCloseTime = (noCRCdata.mid(13,2).toHex().toInt(&ok,16));
    settings<<QString::number(kValue)<<QString::number(rangeMax)
           <<QString::number(connectFrqcyOnOff)<<QString::number(SaveGearsValue)
          <<QString::number(saveTimeInterval)<<QString::number(autoCloseTime);
    qDebug()<<settings;
    return settings;
}

QStringList* PortAgent::Raw_Data_ID(QByteArray *rec)
{
//    QString id = QString::number(rec->mid(2,1).toHex().toInt(&ok,16));
//    return IDLIST;
}

void PortAgent::Data_Instance(QByteArray data)
{
    QStringList dataList;
    dataList = Raw_Data_Instance(&data);
    dataList<<recivedTime;
    QStringList s;
    s<<recivedTime<<dataList.at(0)<<dataList.at(1);
    emit readInstanceData(dataList);//把存入数据库的时间同时发给主界面
    emit fakeTimer(ORDER_READ_INSTANCE_DATA,dataList.at(2).toInt());
    emit writeCsv(s,QDir::currentPath()+"//instance//"+dataList.at(2)+".csv");
    emit addPlotNode(dataList.at(0).toDouble(),dataList.at(1).toDouble());

}

void PortAgent::Data_History(QByteArray data)
{
    QStringList DataList = Raw_Data_History(&data);
    emit fillTable(DataList);
    for(int i = 2;i<DataList.at(1).toInt();i++){
        QStringList list = DataList.at(i).split(" ");
    }
}

void PortAgent::Data_TimePoint(QByteArray data)
{
    emit addTreeNode(Raw_Data_TimePoint(&data));
}

void PortAgent::Data_Settings(QByteArray data)
{
    emit deviceParameter(Raw_Data_Settings(&data));
    emit interval(Raw_Data_Settings(&data).at(4).toInt());
//    qDebug()<<"raw data:     -----------"<<Raw_Data_Settings(&data).at(4);
}

void PortAgent::Data_ID(QByteArray rec)
{

}



//--------------------以下是错误处理------------------------------
//QString PortAgent::Error_Data(QByteArray* rec){
//    int id = rec->mid(0,1).toInt(ok,16);
//    int fun = rec->mid(1,1).toInt(ok,16) - 16*8;
//    int flag = rec->mid(2,1).toInt(ok,16);
//    switch (flag){
//        case 1:return "错误功能码";break;
//        case 2:return "错误地址";break;
//        case 3:return "错误数据";break;
//        case 6:return "从设备忙";break;
//}
//}




