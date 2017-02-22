#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void PortScan();


signals:
     void settingChanged(QSerialPort *port);
     void getCollectedDataList();
private:
    Ui::Settings *ui;
    QString setting;
    QSerialPort *port;
    bool serialStatus;
private slots:
    void returnPort();
    void cancel();
    void connectPort();
};

#endif // SETTINGS_H
