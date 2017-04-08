#include "deviceparameter.h"
#include "ui_deviceparameter.h"
#include <QDateTime>
DeviceParameter::DeviceParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceParameter)
{
    ui->setupUi(this);
    this->setWindowTitle("仪器参数设置");
    this->setMaximumSize(635,335);
    this->setMinimumSize(635,335);
    ui->label_ID->setText(QString::number(device_ID));
    setWindowModality(Qt::ApplicationModal);
    connect(ui->buttonApplay,SIGNAL(clicked()),this,SLOT(buttonApply_clicked()));
    connect(ui->buttonCancel,SIGNAL(clicked()),this,SLOT(buttonCancle_clicked()));
}

DeviceParameter::~DeviceParameter()
{
    delete ui;
}

void DeviceParameter::buttonApply_clicked()
{
    QString s = "";
    s += QString::number(ui->spinBox_K->value()) + ",";
    s += QString::number(ui->doubleSpinBox_H->value()) + ",";
    s += QString::number(ui->comboBox_G->currentIndex()+1) + ",";

    if(ui->checkBox_F->isChecked())
        s += "1,";
    else
        s += "0,";
    if(ui->checkBox_T->isChecked())
        s += "1#" + QString::number(ui->spinBox_T->value()) + ",";
    else
        s += "0,";
    if(ui->checkBox_OFF->isChecked())
        s += "1#" + QString::number(ui->spinBox_OFF->value()) + ",";
    else
        s += "0,";
    s += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    emit DeviceParameterChanged(s);

    this->close();
}

void DeviceParameter::buttonCancle_clicked()
{
    this->close();
}

void DeviceParameter::setDeviceID(int id)
{
    this->device_ID = id;
    ui->label_ID->setText(QString::number(device_ID));
}

void DeviceParameter::sentAgent(PortAgent *agent)
{
    this->Agent = agent;
    connect(Agent,SIGNAL(deviceParameter(QStringList)),this,SLOT(initView(QStringList)));
}


//目前这里缺点东西。。。等惠中改。。。------------------------------------------------------------------------

void DeviceParameter::initView(QStringList Settings)
{
//    settings<<QString::number(kValue,10)<<QString::number(shift,10)
//    <<QString::number(autoCloseTime,10)<<QString::number(connectFrqcyOnOff,10)
//                                      <<QString::number(saveTimeInterval,10)<<QString::number(rangeMax,10);
    ui->spinBox_K->setValue(Settings.at(0).toInt());
    ui->doubleSpinBox_H->setValue(Settings.at(1).toDouble());
//    ui->comboBox_G->setCurrentIndex(Settings.at(2).);


}

//还有一件事-----------------设置关闭窗口的ignore处理
