#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include <lang.h>

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
    void setLanguage(LANG *language);


signals:
     void settingChanged(QSerialPort *port);
     void getCollectedDataList();
private:
    Ui::Settings *ui;
    QString setting;
    QSerialPort *port;
    bool serialStatus;
    LANG *language;
private slots:
    void returnPort();
    void cancel();
    void connectPort();
    void scanPort();
};

#endif // SETTINGS_H
