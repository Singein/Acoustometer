#ifndef WIDGET_H
#define WIDGET_H

#define ORDER_SHOW_COLLECTED_DATA 0
#define ORDER_UPLOAD_COLLECTED_DATA 1
#define ORDER_START_COLLECTING 2
#define ORDER_STOP_COLLECTING 3
#define ORDER_CHANGE_SETTINGS 4
#define ORDER_GET_SETTINGS 5

#include <QWidget>
#include <QStandardItemModel>
#include <QtSerialPort\QtSerialPort>
#include <QtSerialPort\QSerialPortInfo>
#include <QAction>
#include <portagent.h>
#include <settings.h>
#include <deviceparameter.h>
#include <vector>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void portConnect();
    void superShow();

private:
    Ui::Widget *ui;
     QStandardItemModel *model;
     QStandardItem *devices;
     QAction *action_port_setting;
     QAction *action_device_setting;
     PortAgent *portAgent;
     Settings *setDialog;
     DeviceParameter *deviceSettingDialog;
     QSerialPort *port;

     int get_device_address();

     void initTable();
     void viewInit();
     bool isStarted;

signals:
     void itemCheckStatusChanged(QString s);
private slots:
     void initTree(QString s);
     void on_treeView_customContextMenuRequested(const QPoint &pos);
     void settings_changed(QSerialPort *Port);
     void device_setting_changed(QString s);
     void setting_Dialog_Show();
     void device_setting_Dialog_Show();
     void start_and_stop_collecting();
     void getDevicesList();
     void currentIndexChanged(QModelIndex currentIndex);
     void read_history_data(QString s);
};

#endif // WIDGET_H
