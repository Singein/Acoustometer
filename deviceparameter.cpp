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
    s += QString::number(ui->comboBox_G->currentIndex()) + ",";

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
