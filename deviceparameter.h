#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

#include <QWidget>
#include <QButtonGroup>
#include <portagent.h>
#include <lang.h>

namespace Ui {
class DeviceParameter;
}

class PortAgent;

class DeviceParameter : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceParameter(QWidget *parent = 0);
    ~DeviceParameter();
    void setDeviceID(int id);
    void sentAgent(PortAgent *agent);
    void setLanguage(LANG *language);

private:
    Ui::DeviceParameter *ui;
    int device_ID;
    PortAgent *Agent;
    LANG *language;

signals:
    void DeviceParameterChanged(QString s);
    void instance_t(int t);


private slots:
    void buttonApply_clicked();
    void buttonCancle_clicked();
    void initView(QStringList Settings);
};

#endif // DEVICEPARAMETER_H
