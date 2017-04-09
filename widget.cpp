#include "widget.h"
#include "ui_widget.h"
#include <QList>
#include <QTreeWidgetItem>
#include <QMenu>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qDebug()<<"当前线程ID:"<<QThread::currentThreadId();
    ui->setupUi(this);
    map_point = &map;
    isAllDeviceWorking = false;
    viewInit();
    setDialog = new Settings;
    portThread = new QThread;
    deviceSettingDialog = new DeviceParameter;
    portAgent = new PortAgent; //port在 get_devices_list函数中获取，先进行无参的构造是为了下面的connect函数建立
    portAgent->setMap(map_point);
    deviceSettingDialog->sentAgent(portAgent);
    connect(setDialog,SIGNAL(settingChanged(QSerialPort*)),this,SLOT(port_setting_changed(QSerialPort*))); //程序一运行，第一个触发的连接
    connect(setDialog,SIGNAL(getCollectedDataList()),this,SLOT(get_devices_list()));//第二个触发的连接，并设置了portAgent的port参数
    connect(deviceSettingDialog,SIGNAL(DeviceParameterChanged(QString)),this,SLOT(device_setting_changed(QString)));//当任意仪器的参数发生改变时触发
    connect(ui->Button_start,SIGNAL(clicked()),this,SLOT(start_and_stop_collecting()));//实时数据采集 点击开始采集按钮后触发
    connect(ui->Button_import,SIGNAL(clicked()),this,SLOT(import_to_excel()));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(current_index_changed(QModelIndex)));//当树状列表上的节点被点击后触发，用来限定操作逻辑
    connect(portAgent,SIGNAL(addTreeNode(QStringList)),this,SLOT(initTree(QStringList)));//当有列表数据收到后触发
    connect(portAgent,SIGNAL(readInstanceData(QStringList)),this,SLOT(update_instance_data(QStringList)));//更新当前的实时数据
    connect(this,SIGNAL(itemCheckStatusChanged(QString)),this,SLOT(read_history_data(QString)));//这个用来判断树状表中节点状态变化，槽函数 没想好怎么写
}

Widget::~Widget()
{
    delete ui;
}

void Widget::current_index_changed(QModelIndex currentIndex)
{
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    QString s = currentItem->text();
    isInstance = false;
    if(s == "设备列表")
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
    }
    if(s.contains("测量仪"))
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
//        current_modId = s.split(" ").at(1);
        ui->label->setText("当前选中："+s);
    }
    if(s.contains("实时数据"))
    {
        ui->Button_import->show();
        ui->Button_start->show();
        if(portAgent->DB->isTableExist(QString::number(get_device_id())+"_instance"))
            fill_table_all(portAgent->DB->queryDataTableAll(QString::number(get_device_id())+"_instance"));
        isInstance = true;
        if(s.contains("正在采集"))
        {
            ui->Button_start->setText("停止采集");
            ui->Button_import->setEnabled(false);
        }
        else
        {
            ui->Button_start->setText("开始采集");
            ui->Button_import->setEnabled(true);
        }

        /*-------------------------------------------------------------*/
        qDebug()<<portAgent->map->value(get_device_id_toString());

    }
    if(s == "历史数据")
    {
        ui->Button_start->hide();
        ui->Button_import->show();
        ui->Button_import->setEnabled(true);
        if(get_current_item()->checkState()==0)
        {
            get_current_item()->setCheckState(Qt::CheckState::Checked);
            for(int i=0;i<get_current_item()->rowCount();i++)
            {
                QStandardItem *currentItem = get_current_item()->child(i);
                currentItem->setCheckState(Qt::CheckState::Checked);
                portAgent->Set_timeId(currentItem->text());
                portAgent->GiveOrders(ORDER_UPLOAD_HISTORY_DATA,get_device_id());
//                QThread::msleep(10000);
            }
            ui->treeView->expand(ui->treeView->currentIndex());
        }
        else
        {
            get_current_item()->setCheckState(Qt::CheckState::Unchecked);
            for(int i=0;i<get_current_item()->rowCount();i++)
            {
                QStandardItem *currentItem = get_current_item()->child(i);
                currentItem->setCheckState(Qt::CheckState::Unchecked);
            }
        }
    }

    if(s.contains("-"))
    {
        ui->Button_start->hide();
        ui->Button_import->show();
        if(get_current_item()->checkState()==0)
        {
            get_current_item()->setCheckState(Qt::CheckState::Checked);
        }
        else
        {
            get_current_item()->setCheckState(Qt::CheckState::Unchecked);
            get_current_item()->parent()->setCheckState(Qt::CheckState::Unchecked);
        }
    }

    if(currentItem->checkState()==Qt::CheckState::Checked)
    {
        emit itemCheckStatusChanged(s);
    }

}

void Widget::initTree(QStringList nodes)
{   
//    groupCount ++;

//    int modId = nodes.at(0).toInt();
//    int groups = nodes.at(2).toInt();
//    int timeInterval = nodes.at(4).toInt();

//    timeGroupTab[modId][0] ++;
//    timeGroupTab[modId][1] = timeInterval;


//    QDateTime time = QDateTime::fromString(nodes.at(1),"yyyy-MM-dd hh:mm:ss");


//    map->insert(nodes.at(0).toInt(),false);
    QStandardItem *device = new QStandardItem("测量仪 "+nodes.at(0)); //这条是id，设备
    map.insert(nodes.at(0),0);
    QStandardItem *instance_data = new QStandardItem("实时数据");//这条是实时数据
    QStandardItem *history_data = new QStandardItem("历史数据");//这条是历史数据
    device->setEditable(false);
    instance_data->setEditable(false);
    history_data->setEditable(false);
    history_data->setCheckable(true);

    for(int i = 1;i < nodes.length();i++)
    {   
        //逐条添加历史数据时间点
        QStandardItem *node = new QStandardItem(nodes.at(i));
        node->setEditable(false);
        node->setCheckable(true);
        history_data->appendRow(node);
    }
    device->appendRow(instance_data);
    device->appendRow(history_data);
    devices->appendRow(device);
    model->appendRow(devices);//刷新modle
    ui->treeView->setModel(model);//刷新treeview

    qDebug()<<"设备列表和设备时间组列表生成";
}

void Widget::initTable()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(1);
    QStringList tableHeader;
    tableHeader <<"日期/时间"<<"声强值(W/CM2)"<<"频率(KHz)";
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionsMovable(true);
    //----------------------测试--------------------------
    QStringList items;
    items<<"2017-2-27 12:18:56"<<"23"<<"56";
//    for(int i=0;i<20;i++)
//    {

//        add_table_row(items);
//    }
    //-----------------------------------------------------
    qDebug()<<"表格初始化成功";
}

void Widget::initTree_test()
{
    //---------以下仅做测试-----------------
    for(int i=1;i<247;i++)
    {
        map.insert(QString::number(i),0);
        QStandardItem *device = new QStandardItem("测量仪 "+QString::number(i));
        QStandardItem *instance_data = new QStandardItem("实时数据");
        QStandardItem *history_data = new QStandardItem("历史数据");
        QStandardItem *item1 = new QStandardItem("2016-12-17 12:11");
        QStandardItem *item2 = new QStandardItem("2016-12-17 13:00");
        QStandardItem *item3 = new QStandardItem("2016-12-17 13:11");
        item1->setCheckable(true);
        item2->setCheckable(true);
        item3->setCheckable(true);
//        history_data->setCheckable(true);
        history_data->appendRow(item1);
        history_data->appendRow(item2);
        history_data->appendRow(item3);
        device->appendRow(instance_data);
        device->appendRow(history_data);
        devices->appendRow(device);
    }
    qDebug()<<"测试子结点生成";
    //-----------------------------------------
}

void Widget::viewInit()
{  
    this->setWindowTitle("声强检测仪");
    model = new QStandardItemModel (ui->treeView);
    ui->treeView->setMaximumWidth(250);
    devices = new QStandardItem("设备列表");
    devices->setEditable(false);
    ui->Button_import->hide();
    ui->Button_start->hide(); 
    //------------------------------------------------
    qDebug()<<"-----------声强检测仪输出日志--------------";
    qDebug()<<"树状列表根结点初始化成功";
//    initTree_test();
    model->appendRow(devices);
    ui->treeView->setModel(model);
    initTable();
    qDebug()<<"界面初始化完毕";
}

void Widget::fill_table_all(QStringList s)
{
    ui->tableWidget->clear();
    initTable();
    for(int i = 0;i<s.length();i++)
    {
        QStringList items = s.at(i).split(" ");
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(items[0]));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(items[1]));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(items[2]));
        ui->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->item(i, 1)->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->item(i, 2)->setTextAlignment(Qt::AlignHCenter);
    }
}

void Widget::add_table_row(QStringList items)
{
    int rowCount = ui->tableWidget->rowCount()-1;
//    qDebug()<<"成功调用add_table_row!";
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(items.at(items.length()-1)));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(items.at(1)));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(items.at(2)));
//    qDebug()<<ui->tableWidget->rowCount();
    ui->tableWidget->item(rowCount, 0)->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->item(rowCount, 1)->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->item(rowCount, 2)->setTextAlignment(Qt::AlignHCenter);
    ui->tableWidget->setRowCount(rowCount+2);
}

void Widget::update_instance_data(QStringList s)
{
    if(s.at(0) == get_device_id_toString() && isInstance)
        add_table_row(s);
}

void Widget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QStandardItemModel *model = static_cast <QStandardItemModel*>(ui->treeView->model());
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    action_port_setting = new QAction("串口设置");
    action_device_setting = new QAction("仪器设置");
    action_start_stop_All = new QAction;
    connect(action_port_setting,SIGNAL(triggered(bool)),this,SLOT(setting_Dialog_Show()));
    connect(action_device_setting,SIGNAL(triggered(bool)),this,SLOT(device_setting_Dialog_Show()));
    connect(action_start_stop_All,SIGNAL(triggered(bool)),this,SLOT(start_stop_all()));
    if(currentItem->text() == "设备列表")
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        if(!isAllDeviceWorking)
        {
            action_start_stop_All->setText("全部设备开始采集");
//            isAllDeviceWorking = true;
        }
        else
        {
            action_start_stop_All->setText("全部设备停止采集");
//            isAllDeviceWorking = false;
        }
        popMenu->addAction(action_port_setting);//往菜单内添加QAction
        popMenu->addAction(action_start_stop_All);
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

    if(currentItem->text().contains("测量仪"))
    {
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(action_device_setting);//往菜单内添加QAction
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

}

void Widget::port_setting_changed(QSerialPort* Port)
{
    this->port = Port;
    ui->label->setText(port->portName()+" "+QString::number(port->baudRate())+" "+QString::number(port->dataBits())+" "+QString::number(port->stopBits()));
}

void Widget::device_setting_changed(QString s)
{
    ui->label->setText(s);
    portAgent->Set_Settings(s);
    portAgent->GiveOrders(ORDER_CHANGE_SETTINGS,get_device_id());
}

void Widget::setting_Dialog_Show()
{
    setDialog->show();   
}

void Widget::device_setting_Dialog_Show()
{
    deviceSettingDialog->setDeviceID(get_device_id());
    portAgent->GiveOrders(ORDER_GET_SETTINGS,get_device_id());
    deviceSettingDialog->show();
}

int Widget::get_device_id()
{
    QStandardItem *currentItem = get_current_item();
    int index = 0;
    QString s = currentItem->text();
    if(s.contains("测量仪"));
    else
        s = currentItem->parent()->text();
    if(s == "历史数据"|| s == "实时数据")
        s = currentItem->parent()->parent()->text();
    index = s.split(" ").at(1).toInt();
//    ui->label->setText(QString::number(index));
    return index;
}

QString Widget::get_device_id_toString()
{

    QStandardItem *currentItem = get_current_item();
    int index = 0;
    QString s = currentItem->text();
    if(s.contains("测量仪"));
    else
        s = currentItem->parent()->text();
    if(s == "历史数据"|| s == "实时数据")
        s = currentItem->parent()->parent()->text();
   return s.split(" ").at(1);
}

void Widget::start_and_stop_collecting()
{
    if(map.value(get_device_id_toString())==0)
    {
        ui->Button_start->setText("停止采集");
        map.insert(QString::number(get_device_id()),1);
        ui->Button_import->setEnabled(false);
        get_current_item()->setText("实时数据 ==> 正在采集");
        portAgent->GiveOrders(ORDER_START_COLLECTING,get_device_id());
    }
    else
    {
        map.insert(QString::number(get_device_id()),0);
        ui->Button_start->setText("开始采集");
        ui->Button_import->setEnabled(true);
        get_current_item()->setText("实时数据");
        portAgent->GiveOrders(ORDER_STOP_COLLECTING,get_device_id());
    }
}

void Widget::start_stop_all()
{
    if(!isAllDeviceWorking)
    {
        ui->treeView->expand(ui->treeView->currentIndex());
        for (QMap<QString, int>::const_iterator it = map.cbegin(), end = map.cend(); it != end; ++it) {
            map.insert(it.key(),1);
         }

       for(int i=0;i<devices->rowCount();i++)
       {
            QStandardItem *currentItem = devices->child(i);
            ui->treeView->expand(ui->treeView->currentIndex().child(i,0));
            currentItem->child(0)->setText("实时数据 ==> 正在采集");
       }
        isAllDeviceWorking = true;
    }
    else
    {
        for (QMap<QString, int>::const_iterator it = map.cbegin(), end = map.cend(); it != end; ++it) {
            map.insert(it.key(),0);
         }
        for(int i=0;i<devices->rowCount();i++)
        {
             QStandardItem *currentItem = devices->child(i);
             currentItem->child(0)->setText("实时数据");
        }
        isAllDeviceWorking = false;
    }
}

QStandardItem* Widget::get_current_item()
{
    QStandardItemModel *model = static_cast <QStandardItemModel*> (ui->treeView->model());
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    return currentItem;
}

void Widget::super_show()
{
    this->show();
    setDialog->show();
}

void Widget::get_devices_list()
{
    portAgent->setPort(port);
    portAgent->GiveOrders(ORDER_GET_DEVICE_LIST,0);
    portThread->start();

    qDebug()<<"获取设备列表请求已转交 port agent";
}

void Widget::read_history_data(QString s)
{
    //查询相应的
    ui->label->setText(s);
}

void Widget::import_to_excel()
{
    qDebug()<<"import_to_excel called！";
    qDebug()<<ui->tableWidget->rowCount();
    QStringList datalist;


    for(int i=0;i<ui->tableWidget->rowCount()-1;i++)
    {
        QString s="";
        for(int j=0;j<3;j++)
        {
//            if(j==2)
//                s += ui->tableWidget->item(i,j)->text();
//            else
                s += (ui->tableWidget->item(i,j)->text()+",");
        }
        datalist.append(s);
    }
    qDebug()<<datalist.at(0);
    ToExcel *toexcel = new ToExcel;
    toexcel->Import(get_device_id_toString()+QDateTime::currentDateTime().toString("yyMMddhhmmss"),datalist);
}
