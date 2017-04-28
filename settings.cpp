#include "settings.h"
#include "ui_settings.h"
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setMaximumSize(300,240);
    this->setMinimumSize(300,240);
    setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("串口设置");
    ui->comboBox_Baud->setCurrentIndex(7);
    ui->comboBox_CheckBit->setCurrentIndex(2);
    ui->comboBox_StopBit->setCurrentIndex(0);
    ui->comboBox_DataBit->setCurrentIndex(3);
    port = new QSerialPort;
    serialStatus = false;
    PortScan();
    connect(ui->Button_OK,SIGNAL(clicked()),this,SLOT(returnPort()));
    connect(ui->OpenButton,SIGNAL(clicked()),this,SLOT(connectPort()));//发送数据槽连接
}

Settings::~Settings()
{
    delete ui;
}

void Settings::returnPort()
{
    emit settingChanged(port);
    qDebug()<<"串口连接完毕，串口指针已发送";
    emit getCollectedDataList();
    qDebug()<<"获取列表请求已发送，即将初始化树状列表";
    this->close();
}

void Settings::cancel()
{
    this->close();
}

void Settings::PortScan()//扫描串口
{
    ui->comboBox_Port->clear();
    QList<QSerialPortInfo> coms = QSerialPortInfo::availablePorts();
    for(int i = 0;i < coms.length();i++)
    {
        ui->comboBox_Port->addItem(coms.at(i).portName());
    }
}

void Settings::connectPort()//连接串口
{
    int baudRate = ui->comboBox_Baud->currentIndex();
    int checkBit = ui->comboBox_CheckBit->currentIndex();
    int dataBit = ui->comboBox_DataBit->currentIndex();
    int stopBit = ui->comboBox_StopBit->currentIndex();

    if(serialStatus == false)
    {
        port->setPortName(ui->comboBox_Port->currentText());
        switch (baudRate) {
        case 0:port->setBaudRate(QSerialPort::Baud1200);break;
        case 1:port->setBaudRate(QSerialPort::Baud2400);break;
        case 2:port->setBaudRate(QSerialPort::Baud4800);break;
        case 3:port->setBaudRate(QSerialPort::Baud9600);break;
        case 4:port->setBaudRate(QSerialPort::Baud19200);break;
        case 5:port->setBaudRate(QSerialPort::Baud38400);break;
        case 6:port->setBaudRate(QSerialPort::Baud57600);break;
        case 7:port->setBaudRate(QSerialPort::Baud115200);break;
        default:break;
        }

        switch (dataBit) {
        case 0:port->setDataBits(QSerialPort::Data5);break;
        case 1:port->setDataBits(QSerialPort::Data6);break;
        case 2:port->setDataBits(QSerialPort::Data7);break;
        case 3:port->setDataBits(QSerialPort::Data8);break;
        default:break;
        }

        switch (stopBit) {
        case 0:port->setStopBits(QSerialPort::OneStop);break;
        case 1:port->setStopBits(QSerialPort::OneAndHalfStop);break;
        case 2:port->setStopBits(QSerialPort::TwoStop);break;
        default:break;
        }

        switch (checkBit) {
        case 0:port->setParity(QSerialPort::NoParity);break;
        case 1:port->setParity(QSerialPort::OddParity);break;
        case 2:port->setParity(QSerialPort::EvenParity);break;
        case 3:port->setParity(QSerialPort::SpaceParity);break;
        case 4:port->setParity(QSerialPort::MarkParity);break;
        default:break;
        }

        if(!port->open(QIODevice::ReadWrite))
        {
            serialStatus = false;
            QMessageBox::information(this,"Error",port->errorString());
        }
        else
        {
            serialStatus = true;
            ui->comboBox_Baud->setEnabled(false);
            ui->comboBox_CheckBit->setEnabled(false);
            ui->comboBox_DataBit->setEnabled(false);
            ui->comboBox_Port->setEnabled(false);
            ui->comboBox_StopBit->setEnabled(false);
            ui->OpenButton->setText("断开连接");
        }
    }

    else
    {
        if(port->isOpen())
        {
            port->close();
            serialStatus = false;
            ui->comboBox_Baud->setEnabled(true);
            ui->comboBox_CheckBit->setEnabled(true);
            ui->comboBox_DataBit->setEnabled(true);
            ui->comboBox_Port->setEnabled(true);
            ui->comboBox_StopBit->setEnabled(true);
            ui->OpenButton->setText("打开串口");
        }
    }
}
