#include "portagent.h"
#include "QDebug"

PortAgent::PortAgent(QSerialPort *Port)
{
    this->port = Port;

}

PortAgent::PortAgent()
{
    ReciveDataThread = new QThread;
    OperateDataThread = new QThread;
}

void PortAgent::setPort(QSerialPort *p)
{
    this->port = p;
    moveToThread(ReciveDataThread);
    connect(port,SIGNAL(readyRead()),this,SLOT(OrderExcuted()),Qt::QueuedConnection);//消息队列模式
    ReciveDataThread->start();
}

PortAgent::~PortAgent()
{

}

void PortAgent::GiveOrders(int order,int id)
{
    switch (order) {
    case ORDER_SHOW_COLLECTED_DATA:Order_Show_Collected_Data(id);break;
    case ORDER_UPLOAD_SELECTED_DATA:Order_Upload_Selected_Data(id);break;
    case ORDER_START_COLLECTING:Order_Start_Read_Instance(id);break;
    case ORDER_STOP_COLLECTING:Order_Stop_Read_Instance(id);break;
    case ORDER_CHANGE_SETTINGS:Order_Change_Settings(id);break;
    case ORDER_GET_SETTINGS:Order_Get_Settings(id);break;
    case ORDER_READ_INSTANCE_DATA:Order_Read_Instance_Data(id);break;
    default:break;
    }
}

void PortAgent::OrderExcuted()
{
    moveToThread(OperateDataThread);
    QByteArray data = port->readAll();
    //TODO:operation the data
    //第一步取到第一位 机器的ID
    //第二部判断功能码+寄存器，如果功能码是用来读取历史数据时间组的，把解析后的数据以QString格式发送
    //emit addTreeNode(s);
    //如果是实时数据，往数据库里扔，扔完然后发信号
    //emit readInstanceData();
    //如果是历史数据，直接往数据库里扔
    OperateDataThread->start();

}



QString PortAgent::Order_Get_Settings(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Get_Settings called";
    //TODO: build the command message
    //TODO: sender->write(message)
    QString readDataRequest;
    QString modId = QString::number(id,16);
    if(modId.length()<2){
    modId = zero.append(modId);
    }
        readDataRequest.append(modId);
        readDataRequest.append("03");    //功能码
        readDataRequest.append("00");    //寄存器地址高位
        readDataRequest.append("00");    //寄存器地址低位
        readDataRequest.append("00");    //寄存器数目高位
        readDataRequest.append("07");    //寄存器数目低位 (这里我分开写清楚点 内啥觉得太长就并成一句好了)
        QByteArray s = QByteArray::fromHex(readDataRequest.toLatin1().data());
        uint CRCCode = qChecksum(s,readDataRequest.length());  //生成crc码
        readDataRequest.append(CRCCode);
        return readDataRequest;
}

QString PortAgent::Order_Change_Settings(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Change_Settings called";
    //TODO: build the command message
    //TODO: sender->write(message)

       QStringList settings = this->settings.split(",");
       QString modId = QString::number(id,16);
       QString year;
       QString month;
       QString day;
       QString hour;
       QString minute;
       QString second;
       QString temp;
       QStringList timeSettings;
       QStringList time;
       QStringList dateTime;
       QStringList clockTime;
       QString zero = "0";
       bool ok;
       for(int it = 0;it!=6;++it){
           if(settings[it].indexOf("#") == EOF && settings[it].indexOf("-") == EOF){
               temp = QString::number(settings[it].toInt(&ok,16));
               while(temp.length()<4){
                   temp = zero.append(temp);
               }
               settings[it] = temp;
           }else if(settings[it].indexOf("#")!=EOF){
               timeSettings = settings[it].split("#");
               QString temp = QString::number(timeSettings[1].toInt(&ok,16));
               while(temp.length()<2){
                   temp = zero.append(temp);
               }
               settings[it] = temp;
           }else if(settings[it].indexOf("-")!=EOF){
               time = settings[6].split(" ");
               dateTime = time[0].split("-");
               clockTime = time[1].split(":");
               year = QString::number(dateTime[0].toInt(&ok,16));
               if(year.length()==3){
                   year = zero.append(year);
               }
               month = QString::number(dateTime[1].toInt(&ok,16));
               while(month.length()<4){month = zero.append(month);}
               day = QString::number(dateTime[2].toInt(&ok,16));
               while(day.length()<4){day = zero.append(day);}
               hour = QString::number(clockTime[0].toInt(&ok,16));
               while(hour.length()<4){hour = zero.append(hour);}
               minute = QString::number(clockTime[1].toInt(&ok,16));
               while(minute.length()<4){minute = zero.append(minute);}
               second = QString::number(clockTime[2].toInt(&ok,16));
               while(second.length()<4){second = zero.append(second);} //补足一个字的长度
           }
       }
       QString changeDataRequest;
       modId = QString::number(id,16);
       while(modId.length()<4){modId = zero.append(modId);}
       changeDataRequest.append(id);
       changeDataRequest.append("0000");  //寄存器地址
       changeDataRequest.append("0010");  //寄存器数目
       changeDataRequest.append("20");  //字节数
       changeDataRequest.append(settings[0]);  //寄存器[0]位k值
       changeDataRequest.append(settings[2]);   //档位 寄存器
       changeDataRequest.append(settings[5]);  //自动关机时间
       changeDataRequest.append(settings[3]);  //频率关联状态
       changeDataRequest.append(settings[4]); //存储时间间隔
       changeDataRequest.append(settings[1]); //测量量程最大值
       changeDataRequest.append(modId);    //485id
       changeDataRequest.append(year);
       changeDataRequest.append(month);
       changeDataRequest.append(day);
       changeDataRequest.append(hour);
       changeDataRequest.append(minute);
       changeDataRequest.append(second);
       QByteArray s = QByteArray::fromHex(changeDataRequest.toLatin1().data());
       uint CRCCode = qChecksum(s,changeDataRequest.length());
       changeDataRequest.append(CRCCode);
       return changeDataRequest;
}

QStringList PortAgent::Order_Show_Collected_Data(int id)
{

//    QString test = QString::number(id,16)+"03050000000000";
//    QByteArray s = QByteArray::fromHex(test.toLatin1().data());
//    qDebug()<<s.length();
//    qDebug()<<s.size();
//    if(port->write(s)>0);
         qDebug()<<"MODID:"<<id<<" "<<"Order_Show_Collected_Data called";
    //TODO: build the command message
    //TODO: sender->write(message)
//         QThread::msleep(1);


         QStringList CollectedOrder;

             QString modId = QString::number(id,16);
             if(modId.length()<2){
                 modId = zero.append(modId);
             }

             QString readTimeGroupRequestChangeReg;
             readTimeGroupRequestChangeReg.append(modId);
             readTimeGroupRequestChangeReg.append("06");
             readTimeGroupRequestChangeReg.append("00");
             readTimeGroupRequestChangeReg.append("10");
             readTimeGroupRequestChangeReg.append("0001");
             QByteArray s1 = QByteArray::fromHex(readTimeGroupRequestChangeReg.toLatin1().data());
             uint CRCCode1 = qChecksum(s1,readTimeGroupRequestChangeReg.length());  //生成crc码
             readTimeGroupRequestChangeReg.append(CRCCode1);

             QString readTimeTableRequestOrder;
             readTimeTableRequestOrder.append(modId);
             readTimeTableRequestOrder.append("03");   //功能码
             readTimeTableRequestOrder.append("0011");   //地址位
             readTimeTableRequestOrder.append("0006");   //寄存器数量
             QByteArray s = QByteArray::fromHex(readTimeTableRequestOrder.toLatin1().data());
             uint CRCCode = qChecksum(s,readTimeTableRequestOrder.length());  //生成crc码
             readTimeTableRequestOrder.append(CRCCode);
             return CollectedOrder<<readTimeGroupRequestChangeReg<<readTimeTableRequestOrder;
}

QStringList PortAgent::Order_Upload_Selected_Data(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Upload_Selected_Data called";
    //TODO: build the command message
    //TODO: sender->write(message)
    QStringList UploadRequest;
        QString modId = QString::number(id,16);
        if(modId.length()<2){
            modId = zero.append(modId);
        }
        QString number = QString::number(id,16);
        while (number.length()<4){
            number = zero.append(number);
        }
        QString readTimeGroupRequestChangeReg1;
        readTimeGroupRequestChangeReg1.append(modId);
        readTimeGroupRequestChangeReg1.append("06");
        readTimeGroupRequestChangeReg1.append("00");
        readTimeGroupRequestChangeReg1.append("17");
        readTimeGroupRequestChangeReg1.append(number);
        QByteArray s1 = QByteArray::fromHex(readTimeGroupRequestChangeReg1.toLatin1().data());
        uint CRCCode1 = qChecksum(s1,readTimeGroupRequestChangeReg1.length());  //生成crc码
        readTimeGroupRequestChangeReg1.append(CRCCode1);

        QString readTimeGroupRequestChangeReg2;
        readTimeGroupRequestChangeReg2.append(modId);
        readTimeGroupRequestChangeReg2.append("06");
        readTimeGroupRequestChangeReg2.append("00");
        readTimeGroupRequestChangeReg2.append("16");
        readTimeGroupRequestChangeReg2.append("0001");
        QByteArray s2 = QByteArray::fromHex(readTimeGroupRequestChangeReg2.toLatin1().data());
        uint CRCCode2 = qChecksum(s2,readTimeGroupRequestChangeReg2.length());  //生成crc码
        readTimeGroupRequestChangeReg2.append(CRCCode2);

        QString readTimeGroupRequest;
        readTimeGroupRequest.append(QString::number(id,16));
        readTimeGroupRequest.append("03");    //功能码
        readTimeGroupRequest.append("00");    //寄存器地址高位
        readTimeGroupRequest.append("17");    //寄存器地址低位
        readTimeGroupRequest.append("00");    //寄存器数目高位
        readTimeGroupRequest.append("02");    //寄存器数目低位 (这里我分开写清楚点 内啥觉得太长就并成一句好了)
        QByteArray s = QByteArray::fromHex(readTimeGroupRequest.toLatin1().data());
        uint CRCCode = qChecksum(s,readTimeGroupRequest.length());  //生成crc码
        readTimeGroupRequest.append(CRCCode);

        UploadRequest<<readTimeGroupRequestChangeReg1<<readTimeGroupRequestChangeReg2<<readTimeGroupRequest;
        return UploadRequest;
}

QString PortAgent::Order_Read_Instance_Data(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Read_Instance_Data called";
    QString readDataRequest;
       QString modId = QString::number(id,16);
       if(modId.length()<2){modId = zero.append(modId);}
       readDataRequest.append(modId);
       readDataRequest.append("03");    //功能码
       readDataRequest.append("00");    //寄存器地址高位
       readDataRequest.append("08");    //寄存器地址低位
       readDataRequest.append("00");    //寄存器数目高位
       readDataRequest.append("08");    //寄存器数目低位 (这里我分开写清楚点 内啥觉得太长就并成一句好了)
       QByteArray s = QByteArray::fromHex(readDataRequest.toLatin1().data());
       uint CRCCode = qChecksum(s,readDataRequest.length());  //生成crc码
       readDataRequest.append(CRCCode);
       return readDataRequest;
}

void PortAgent::Order_Start_Read_Instance(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Start_Collecting called";
    //TODO: build the command message
    //TODO: sender->write(message)
}

void PortAgent::Order_Stop_Read_Instance(int id)
{
    qDebug()<<"MODID:"<<id<<" "<<"Order_Stop_Collecting called";
    //TODO: build the command message
    //TODO: sender->write(message)
}

QStringList PortAgent::Data_Instance(QByteArray rec)
{
    QByteArray noCRCdata = rec.mid(0,19);
        QStringList instanceData;
        int modid = (noCRCdata.mid(0,1).toInt(&ok,16));
        int fun = noCRCdata.mid(1,1).toInt(&ok,16);
        if(fun == 03 ){
            bool ok;
            int num = (noCRCdata.mid(2,1).toInt(&ok,16));
            int year = (noCRCdata.mid(3,2).toInt(&ok,16));
            int month = (noCRCdata.mid(5,2).toInt(&ok,16));
            int day = (noCRCdata.mid(7,2).toInt(&ok,16));
            int hour = (noCRCdata.mid(9,2).toInt(&ok,16));
            int minute = (noCRCdata.mid(11,2).toInt(&ok,16));
            int second = (noCRCdata.mid(13,2).toInt(&ok,16));
            int vol = (noCRCdata.mid(15,2).toInt(&ok,16));
            int fre = (noCRCdata.mid(17,2).toInt(&ok,16));
            //QString s = QString::number(year,10);
            //emit operationFinished(QByteArray::number(vol), QByteArray::number(fre));
            instanceData<<QString::number(modid,10)<<QString::number(vol,10)<<QString::number(fre,10);
        }
        return instanceData;
}

QStringList PortAgent::Data_Collected(QByteArray rec)
{
    QByteArray noCRCData = rec.mid(0,15);
    QStringList CollectedData;
    bool ok;
    int modid = noCRCData.mid(0,1).toInt(&ok,16);
    int fun = noCRCData.mid(1,1).toInt(&ok,16);
    if (fun == 03){
        int num = noCRCData.mid(2,1).toInt(&ok,16);
        int year = noCRCData.mid(3,1).toInt(&ok,16);
        int month = noCRCData.mid(4,1).toInt(&ok,16);
        int day = noCRCData.mid(5,1).toInt(&ok,16);
        int hour = noCRCData.mid(6,1).toInt(&ok,16);
        int minute = noCRCData.mid(7,1).toInt(&ok,16);
        int second = noCRCData.mid(8,1).toInt(&ok,16);
        int timeGroupNum = noCRCData.mid(9,2).toInt(&ok,16);
        int startPosition = noCRCData.mid(11,2).toInt(&ok,16);
        int timeInterval = noCRCData.mid(13,2).toInt(&ok,16);
        CollectedData<<QString::number(year,10)<<QString::number(month,10)<<QString::number(day,10)<<QString::number(hour,10)<<QString::number(minute,10)<<QString::number(second,10)<<QString::number(timeGroupNum,10)<<QString::number(startPosition,10)<<QString::number(timeInterval,10);
    }
    return CollectedData;
}

QStringList PortAgent::Data_Selected(QByteArray rec)
{
    QByteArray noCRCCode = rec.mid(0,7);
        QStringList SelectedData;
        int modId = noCRCCode.mid(0,1).toInt(&ok,16);
        int fun = noCRCCode.mid(1,1).toInt(&ok,16);
        if (fun == 03 ){
            int num = noCRCCode.mid(2,1).toInt(&ok,16);
            int soundGroup = noCRCCode.mid(3,2).toInt(&ok,16);
            int freGroup = noCRCCode.mid(5,2).toInt(&ok,16);
            SelectedData<<QString::number(soundGroup,10)<<QString::number(freGroup,10);
        }
        return SelectedData;
}

QStringList PortAgent::Data_Settings(QByteArray rec)
{
    QByteArray noCRCdata = rec.mid(0,17);
        QStringList settings;
        int modid = (noCRCdata.mid(0,1).toInt(&ok,16));
        int fun = noCRCdata.mid(1,1).toInt(&ok,16);
        if(fun == 03 ){
            bool ok;
            int num = (noCRCdata.mid(2,1).toInt(&ok,16));
            int kValue = (noCRCdata.mid(3,2).toInt(&ok,16));
            int shift = (noCRCdata.mid(5,2).toInt(&ok,16));
            int autoCloseTime = (noCRCdata.mid(7,2).toInt(&ok,16));
            int connectFrqcyOnOff = (noCRCdata.mid(9,2).toInt(&ok,16));
            int saveTimeInterval = (noCRCdata.mid(11,2).toInt(&ok,16));
            int rangeMax = (noCRCdata.mid(13,2).toInt(&ok,16));
            int moddid = (noCRCdata.mid(15,2).toInt(&ok,16));
            //QString s = QString::number(year,10);
            //emit operationFinished(QByteArray::number(vol), QByteArray::number(fre));
            settings<<QString::number(kValue,10)<<QString::number(shift,10)<<QString::number(autoCloseTime,10)<<QString::number(connectFrqcyOnOff,10)<<QString::number(saveTimeInterval,10)<<QString::number(rangeMax,10);
        }
        return settings;

}

void PortAgent::Set_Settings(QString Settings)
{
    this->settings = Settings;
}
