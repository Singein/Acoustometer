#include "widget.h"
#include "ui_widget.h"
#include <QList>
#include <QTreeWidgetItem>
#include <QMenu>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    viewInit();
    setDialog = new Settings;
    deviceSettingDialog = new DeviceParameter;
    portAgent = new PortAgent;
    connect(deviceSettingDialog,SIGNAL(DeviceParameterChanged(QString)),this,SLOT(device_setting_changed(QString)));
    connect(setDialog,SIGNAL(settingChanged(QSerialPort*)),this,SLOT(settings_changed(QSerialPort*)));
    connect(ui->Button_start,SIGNAL(clicked()),this,SLOT(start_and_stop_collecting()));
    connect(setDialog,SIGNAL(getCollectedDataList()),this,SLOT(getDevicesList()));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(currentIndexChanged(QModelIndex)));
    connect(portAgent,SIGNAL(addTreeNode(QString)),this,SLOT(initTree(QString)));
//    connect(portAgent,SIGNAL(readInstanceData()),this,SLOT());
    connect(this,SIGNAL(itemCheckStatusChanged(QString)),this,SLOT(read_history_data(QString)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::currentIndexChanged(QModelIndex currentIndex)
{
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    QString s = currentItem->text();
    if(s == "设备列表")
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
    }
    if(s.contains("测量仪"))
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
    }
    if(s == "实时数据")
    {
        ui->Button_import->show();
        ui->Button_start->show();
    }
    if(s == "历史数据")
    {
        ui->Button_start->hide();
        ui->Button_import->show();
    }
}

void Widget::initTree(QString s)
{   
    QStringList nodes = s.split(",");
    QStandardItem *device = new QStandardItem("测量仪#"+nodes.at(0));
    QStandardItem *instance_data = new QStandardItem("实时数据");
    QStandardItem *history_data = new QStandardItem("历史数据");
    device->setEditable(false);
    instance_data->setEditable(false);
    history_data->setEditable(false);
    for(int i = 1;i < nodes.length();i++)
    {   
        QStandardItem *node = new QStandardItem(nodes.at(i));
        node->setEditable(false);
        node->setCheckable(true);
        history_data->appendRow(device);
    }
    device->appendRow(instance_data);
    device->appendRow(history_data);
    model->appendRow(devices);
    ui->treeView->setModel(model);
}

void Widget::initTable()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(0);
    QStringList tableHeader;
    tableHeader <<"日期/时间"<<"声强值(W/CM2)"<<"频率(KHz)";
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionsMovable(true);
}

void Widget::viewInit()
{  
    this->setWindowTitle("声强检测仪");
    model = new QStandardItemModel (ui->treeView);
    ui->treeView->setMaximumWidth(200);
    devices = new QStandardItem("设备列表");
    devices->setEditable(false);
    ui->Button_import->hide();
    ui->Button_start->hide();
    ui->treeView->expandAll();
    //---------以下仅做测试-----------------

    QStandardItem *item1 = new QStandardItem("历史数据");
    item1->setCheckable(true);
    devices->appendRow(item1);

    //-------------------------------
    model->appendRow(devices);
    ui->treeView->setModel(model);
    initTable();
    isStarted = false;
}

void Widget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QStandardItemModel *model = static_cast <QStandardItemModel*>(ui->treeView->model());
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    action_port_setting = new QAction("串口设置");
    action_device_setting= new QAction("仪器设置");
    connect(action_port_setting,SIGNAL(triggered(bool)),this,SLOT(setting_Dialog_Show()));
    connect(action_device_setting,SIGNAL(triggered(bool)),this,SLOT(device_setting_Dialog_Show()));
    if(currentItem->text() == "设备列表")
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(action_port_setting);//往菜单内添加QAction
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

    if(currentItem->text().contains("测量仪"))
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(action_device_setting);//往菜单内添加QAction
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

}

void Widget::settings_changed(QSerialPort* Port)
{
    this->port = Port;
    ui->label->setText(port->portName()+QString::number(port->baudRate())+" "+QString::number(port->dataBits())+" "+QString::number(port->stopBits()));
}

void Widget::device_setting_changed(QString s)
{
    ui->label->setText(s);
}

void Widget::setting_Dialog_Show()
{
    setDialog->show();   
}

void Widget::device_setting_Dialog_Show()
{
    deviceSettingDialog->setDeviceID(get_device_address());
    deviceSettingDialog->show();
}

int Widget::get_device_address()
{
    QStandardItemModel *model = static_cast <QStandardItemModel*> (ui->treeView->model());
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);

    int index = 0;
    QString s = currentItem->parent()->text();
//    if(s.contains("测量仪"))
//        index = s.split("#").at(1).toInt();
    if(s == "历史数据")
        s = currentItem->parent()->parent()->text();

    index = s.split("#").at(1).toInt();
    return index;
}

void Widget::start_and_stop_collecting()
{

    if(isStarted)
    {
        isStarted = false;
        ui->Button_start->setText("开始采集");
        ui->Button_import->setEnabled(true);
    }
    else
    {
        isStarted = true;
        ui->Button_start->setText("停止采集");
        ui->Button_import->setEnabled(false);
        portAgent->GiveOrders(ORDER_START_COLLECTING,get_device_address());
    }
}

void Widget::superShow()
{
    this->show();
    setDialog->show();
}

void Widget::getDevicesList()
{
    portAgent->setPort(port);
    for(int i=1;i<=247;i++)
    {
        portAgent->GiveOrders(ORDER_SHOW_COLLECTED_DATA,i);
    }
}

void Widget::read_history_data(QString s)
{
    //查询相应的
}
