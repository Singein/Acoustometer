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
#include <QMap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
//    void portConnect();
    void super_show(); 
    QStandardItem* get_current_item();
    QMap<QString,int> map;
    QMap<QString,int> *map_point;


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
     void initTable();
     void viewInit();
     void fill_table_all(QStringList s);
     void add_table_row(QStringList items);
     int get_device_id();
     QString get_device_id_toString();
     bool isStarted;
     static int timeGroupTab[246][2];
     int groupCount;

     QString current_modId;
     bool isInstance;




signals:
     void itemCheckStatusChanged(QString s);

private slots:
     void initTree(QStringList nodes);
     void on_treeView_customContextMenuRequested(const QPoint &pos);
     void port_setting_changed(QSerialPort *Port);
     void device_setting_changed(QString s);
     void setting_Dialog_Show();
     void device_setting_Dialog_Show();
     void start_and_stop_collecting();
     void get_devices_list();
     void current_index_changed(QModelIndex currentIndex);
     void read_history_data(QString s);
     void update_instance_data(QStringList s);
};

#endif // WIDGET_H
