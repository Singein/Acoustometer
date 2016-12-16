#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class DeviceParameter;
}

class DeviceParameter : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceParameter(QWidget *parent = 0);
    ~DeviceParameter();
    void setDeviceID(int id);

private:
    Ui::DeviceParameter *ui;
    int device_ID;

signals:
    void DeviceParameterChanged(QString s);


private slots:
    void buttonApply_clicked();
    void buttonCancle_clicked();
};

#endif // DEVICEPARAMETER_H
