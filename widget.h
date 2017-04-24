#ifndef WIDGET_H
#define WIDGET_H

#define ORDER_GET_DEVICE_LIST 0
#define ORDER_UPLOAD_HISTORY_DATA 1
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
#include <toexcel.h>

namespace Ui {
class Widget;
}



class Widget : public QWidget
{
    Q_OBJECT
QThread *portThread;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
//    void portConnect();
    void super_show();  //开始运行时弹出两个主窗口和串口设置窗口
    QStandardItem* get_current_item(); //用来获取当前树状列表选中的item
    QMap<QString,int> map;//每个设备的开关状态表
    QMap<QString,int> *map_point;//上面的变量的指针，用来向portagent传参
    PortAgent *portAgent;//很关键精髓的角色，与下位机的交互全部靠这个



private:
    Ui::Widget *ui;
     QStandardItemModel *model;
     QStandardItem *devices; //设备列表的item
     QAction *action_port_setting;//串口的设置菜单
     QAction *action_device_setting;//设备的参数设置菜单
     QAction *action_start_stop_All;//用来一键开启或关闭所有设备的菜单
//     bool isAllDeviceWorking;
     Settings *setDialog;//串口设置的窗口
     DeviceParameter *deviceSettingDialog;//参数设置窗口
     QSerialPort *port;//串口
     void initTable();//用来初始化表格
     void viewInit();//用来初始化树状列表
     void add_table_row(QStringList items);//一行一行填充
     int get_device_id();//获取当前的设备的id
     QString get_device_id_toString();//同上，区别是返回qstring
     bool isInstance; //判断当前选中是否是在实时数据


signals:
     void itemCheckStatusChanged(QString s); //当历史数据时间组选中状态改变的时候
     void orders(int order,int id);

private slots:
     void initTree_test();
     void initTree(QStringList nodes);//初始化树状列表
     void fill_table_all(QStringList s);//一整张表填充
     void on_treeView_customContextMenuRequested(const QPoint &pos);//右键菜单
     void port_setting_changed(QSerialPort *Port);//串口设置改变
     void device_setting_changed(QString s);//设备的设置改变
     void setting_Dialog_Show();//显示串口设置列表
     void device_setting_Dialog_Show();//显示设备设置列表
     void start_and_stop_collecting();//开始和停止
//     void start_stop_all();
     void get_devices_list();//获取设备列表
     void current_index_changed(QModelIndex currentIndex);//当前选中改变
     void read_history_data(QString s);//读取历史数据
     void update_instance_data(QStringList s);//更新实时数据
     void export_to_excel();

};

#endif // WIDGET_H
