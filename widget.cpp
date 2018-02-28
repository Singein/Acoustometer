#include "widget.h"
#include "ui_widget.h"
#include <QList>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QMenu>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->languageType = 1;
    language = new LANG(0);
    map_point = &map;
    viewInit();
    portSettingDialog = new Settings;
    portSettingDialog->setLanguage(language);
    portThread = new QThread;
    deviceSettingDialog = new DeviceParameter;
    deviceSettingDialog->setLanguage(language);
    plotDialog = new Plot;
    plotDialog->setLanguage(language);
    portAgent = new PortAgent; //port在 get_devices_list函数中获取，先进行无参的构造是为了下面的connect函数建立
    portAgent->setMap(map_point);
    deviceSettingDialog->sentAgent(portAgent);
    connect(portSettingDialog,SIGNAL(settingChanged(QSerialPort*)),this,SLOT(port_setting_changed(QSerialPort*))); //程序一运行，第一个触发的连接
    connect(portSettingDialog,SIGNAL(getCollectedDataList()),this,SLOT(get_devices_list()));//第二个触发的连接，并设置了portAgent的port参数
    connect(deviceSettingDialog,SIGNAL(DeviceParameterChanged(QString)),this,SLOT(device_setting_changed(QString)));//当任意仪器的参数发生改变时触发
    connect(deviceSettingDialog,SIGNAL(instance_t(int)),portAgent,SLOT(setT(int)));
    connect(ui->Button_start,SIGNAL(clicked()),this,SLOT(start_and_stop_collecting()));//实时数据采集 点击开始采集按钮后触发
    connect(ui->Button_import,SIGNAL(clicked()),this,SLOT(export_to_excel()));
    connect(ui->Button_plot,SIGNAL(clicked()),this,SLOT(plot_dialog_show()));
    //connect(ui->Button_getHisPoint,SIGNAL(clicked()),this,SLOT(getHistoryData()));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(current_index_changed(QModelIndex)));//当树状列表上的节点被点击后触发，用来限定操作逻辑
    connect(portAgent,SIGNAL(addTreeNode(QStringList)),this,SLOT(initTree(QStringList)));//当有列表数据收到后触发
    //connect(portAgent,SIGNAL(addDeviceId(QString)),this,SLOT(setDeviceId(QString)));
    connect(portAgent,SIGNAL(addDeviceId(QString)),this,SLOT(initTreePart(QString)));//仅收到列表部分数据后触发
    connect(portAgent,SIGNAL(readInstanceData(QStringList)),this,SLOT(update_instance_data(QStringList)));//更新当前的实时数据
    connect(portAgent,SIGNAL(fillTable(QStringList)),this,SLOT(fill_table_all(QStringList)));
    connect(portAgent->DS,SIGNAL(readyRead(QStringList)),this,SLOT(read_csv(QStringList)));
    connect(portAgent->DS->excel,SIGNAL(current_progress(double)),this,SLOT(set_progressBar_value(double)));
    connect(portAgent,SIGNAL(connectError()),this,SLOT(connectError()));
    connect(portAgent,SIGNAL(falseMessage(int)),this,SLOT(messageError(int)));
    connect(this,SIGNAL(saveAsCsv(QStringList,QString)),portAgent->DS,SLOT(exportExcel(QStringList,QString)));
    connect(this,SIGNAL(orders(int,int)),portAgent,SLOT(GiveOrders(int,int)),Qt::QueuedConnection);
    connect(this,SIGNAL(orders(int,QString)),portAgent,SLOT(GiveOrders(int,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(getInstanceBuff(QString)),portAgent->DS,SLOT(readCsv(QString)));
    connect(this,SIGNAL(plotData(QStringList,QString)),this->plotDialog,SLOT(addNodes(QStringList,QString)));
}

void Widget::setLanguage(int languageType)
{
    this->languageType = languageType;
    this->language = new LANG(languageType);
}

void Widget::load()
{
    QString qss;
    QFile file(":/qss/ui.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        file.close();
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connectError()
{
    QMessageBox::warning(this,language->connectError,language->connectErrorInfo);
    this->portAgent->orderList.clear();
}

void Widget::messageError(int id){
    switch(id){
        case 194:QMessageBox::warning(this,language->messageError,language->messageErrorInfoAddress);break;
        case 195:QMessageBox::warning(this,language->messageError,language->messageErrorInfoData);break;
        case 198:QMessageBox::warning(this,language->messageError,language->messageErrorInfoBusy);break;
    }
}

void Widget::current_index_changed(QModelIndex currentIndex)
{
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    QString s = currentItem->text();
    isInstance = false;
    if(s == language->deviceList)
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
        ui->Button_plot->hide();
        ui->label->setText(language->connectedDevices(devices->rowCount()));
    }
    if(s.contains(language->device))
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
        ui->Button_plot->hide();
        ui->label->setText(language->selection+s);
        ui->treeView->expand(ui->treeView->currentIndex());
    }
    if(s.contains(language->realTimeData))
    {
        ui->label->setText(language->formContent(get_device_id(),0));
        ui->Button_import->show();
        ui->Button_start->show();
        ui->Button_plot->show();
        ui->tableWidget->clear();
        initTable();
        emit getInstanceBuff(QDir::currentPath()+"//instance//"+get_device_id_toString()+".csv");

        isInstance = true;
        if(s.contains(language->sampling))
        {
            ui->Button_start->setText(language->buttonStop);
            ui->Button_import->setEnabled(false);
        }
        else
        {
            ui->Button_start->setText(language->buttonStart);
            ui->Button_import->setEnabled(true);
        }
    }
    if(s == language->historyData)
    {
        ui->label->setText(language->formContent(get_device_id(),1));
        ui->Button_start->hide();
        ui->Button_import->show();
        ui->Button_plot->show();
        ui->Button_import->setEnabled(true);
        ui->treeView->expand(ui->treeView->currentIndex());
        ui->tableWidget->clear();
        initTable();
        int j = 0;
        for(int i=0;i<get_current_item()->rowCount();i++)
        {
            QStandardItem *currentItem = get_current_item()->child(i);
            if(currentItem->checkState()==2){
                if(isInstanceDataCollecting()){
                    QMessageBox::warning(this,language->warring,language->warringInfo);
                    return;
                }
                emit orders(get_device_id(),currentItem->text());
                j++;
            }
        }
        if(j==0)
        {
            QStringList s;
            emit plotData(s,language->device+"-"+get_device_id_toString()+" "+language->historyData);
        }
    }

    if(s.contains("-"))
    {
        ui->Button_start->hide();
        ui->Button_import->show();
        ui->Button_plot->show();
        if(isInstanceDataCollecting())
            QMessageBox::warning(this,language->warring,language->warringInfo);
        else{
            ui->tableWidget->clear();
            initTable();
            if(get_current_item()->checkState()==0)
            {
                get_current_item()->setCheckState(Qt::CheckState::Checked);
                for(int i=0;i<get_current_item()->parent()->rowCount();i++)
                {
                    QStandardItem *currentItem = get_current_item()->parent()->child(i);
                    if(currentItem->checkState()==2){
                        emit orders(get_device_id(),currentItem->text());
                    }
                }
            }
            else
            {
                get_current_item()->setCheckState(Qt::CheckState::Unchecked);
                int j = 0;
                for(int i=0;i<get_current_item()->parent()->rowCount();i++)
                {
                    QStandardItem *currentItem = get_current_item()->parent()->child(i);
                    if(currentItem->checkState()==2){
                        j++;
                        emit orders(get_device_id(),currentItem->text());
                    }
                }
                if(j==0)
                {
                    QStringList s;
                    emit plotData(s,language->device+"-"+get_device_id_toString()+" "+language->historyData);
                }
            }

        }
    }
}

void Widget::initTree(QStringList nodes)
{
    int index;
    for(index = 0;index<devices->rowCount();index++){
        devices->removeRow(index);
    }

    for(index=0;index<devices->rowCount();index++)
        if(devices->child(index)->text().contains(nodes.at(0)))
            break;

    if((index==devices->rowCount()-1&&index!=0)||devices->rowCount()==0)
    {
        QStandardItem *device = new QStandardItem(language->device+" "+nodes.at(0));
        map.insert(nodes.at(0),0);
        QStandardItem *instance_data = new QStandardItem(language->realTimeData);
        QStandardItem *history_data = new QStandardItem(language->historyData);
        device->setEditable(true);
        instance_data->setEditable(false);
        history_data->setEditable(false);
        for(int i = 1;i < nodes.length();i++)
        {
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
    }
}

/**
 * @brief Widget::initTreePart
 * 当下位机不存在历史数据节点时触发
 */

void Widget::initTreePart(QString id){
    int index;
    for(index = 0;index<devices->rowCount();index++){
        devices->removeRow(index);
    }
    QStandardItem *device = new QStandardItem(language->device+" "+id);
    QStandardItem *instance_data = new QStandardItem(language->realTimeData);
    device->setEditable(false);
    instance_data->setEditable(false);
    device->appendRow(instance_data);
    devices->appendRow(device);
    model->appendRow(devices);//刷新model
    ui->treeView->setModel(model);//刷新treeview
}

void Widget::initTable()
{
    ui->tableWidget->setColumnCount(3);
    QStringList tableHeader;
    tableHeader <<language->tableTitleDateTime<<language->tableTitleStrength<<language->tableTitleFrequency;
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionsMovable(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    RowCount = 1;
}

void Widget::viewInit()
{  
    this->setWindowTitle(language->applicationTitle);
    ui->progressBar->hide();
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    load();
    model = new QStandardItemModel (ui->treeView);
    ui->treeView->setMaximumWidth(250);
    devices = new QStandardItem(language->deviceList);
    devices->setEditable(false);
    ui->Button_import->hide();
    ui->Button_start->hide(); 
    ui->Button_plot->hide();
    model->appendRow(devices);
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    initTable();
}

QString Widget::count_time(QString time,int i)
{
//    QDateTime t = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss");
//    qDebug()<<"---------------------------"<<history_interval;
    QDateTime t = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss").addSecs((qint64)(history_interval*i));
    return t.toString("yyyy-MM-dd hh:mm:ss");

}

void Widget::fill_table_all(QStringList s)
{
    for(int i = 2;i<s.length();i++)
    {
        QStringList items;
        items<<s.at(i).split(" ").at(0);
        items<<s.at(i).split(" ").at(1);
        items<<s.at(i).split(" ").at(2)+" "+s.at(i).split(" ").at(3);
        add_table_row(items);
    }
    emit plotData(getTableData(),language->device+"-"+get_device_id_toString()+" "+language->historyData);
}

void Widget::read_csv(QStringList s)
{
    if(!s.isEmpty()){
        for(int i=0;i<s.length()-1;i++)
        {
            QStringList items;
            items<<s.at(i).split(",").at(1);
            items<<s.at(i).split(",").at(2);
            items<<s.at(i).split(",").at(0);
            add_table_row(items);
        }
        emit plotData(getTableData(),language->device+"-"+get_device_id_toString()+" "+language->realTimeData);
    }
}

QStringList Widget::getTableData()
{
    QStringList datalist;
    for(int i=0;i<ui->tableWidget->rowCount()-1;i++)
    {
        QString s="";
        for(int j=0;j<3;j++)
        {
            if(j==2)
                s += ui->tableWidget->item(i,j)->text();
            else
                s += (ui->tableWidget->item(i,j)->text()+",");
        }
        datalist.append(s);
    }
    return datalist;
}

void Widget::add_table_row(QStringList items)
{
    ui->tableWidget->setRowCount(RowCount);
    int rowCount = ui->tableWidget->rowCount()-1;

    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(items.at(items.length()-1)));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(items.at(0)));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(items.at(1)));
    ui->tableWidget->item(rowCount, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount, 0)->setTextColor(QColor(187,187,187));
    ui->tableWidget->item(rowCount, 1)->setTextColor(QColor(187,187,187));
    ui->tableWidget->item(rowCount, 2)->setTextColor(QColor(187,187,187));
//    ui->tableWidget->viewport()->setFocusPolicy(Qt::NoFocus);
    RowCount ++;
    ui->tableWidget->setRowCount(rowCount+2);

}

void Widget::update_instance_data(QStringList s)
{
    if(s.at(2) == get_device_id_toString() && isInstance)
    {
        add_table_row(s);
        emit plotData(getTableData(),language->device+"-"+get_device_id_toString()+" "+language->realTimeData);
    }
}

void Widget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QStandardItemModel *model = static_cast <QStandardItemModel*>(ui->treeView->model());
    QModelIndex currentIndex = ui->treeView->currentIndex();
    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    action_port_setting = new QAction(language->portSetting);
    action_device_setting = new QAction(language->deviceSetting);
    action_device_getHistoryData = new QAction(language->getHistoryData);
    action_start_stop_All = new QAction;
    connect(action_port_setting,SIGNAL(triggered(bool)),this,SLOT(port_setting_Dialog_Show()));
    connect(action_device_setting,SIGNAL(triggered(bool)),this,SLOT(device_setting_Dialog_Show()));
    connect(action_device_getHistoryData,SIGNAL(triggered(bool)),this,SLOT(getHistoryData()));
//    connect(action_start_stop_All,SIGNAL(triggered(bool)),this,SLOT(start_stop_all()));
    if(currentItem->text() == language->deviceList)
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
        ui->Button_plot->hide();
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
//        if(!isAllDeviceWorking)
//        {
//            action_start_stop_All->setText("全部设备开始采集");
////            isAllDeviceWorking = true;
//        }
//        else
//        {
//            action_start_stop_All->setText("全部设备停止采集");
////            isAllDeviceWorking = false;
//        }
        popMenu->addAction(action_port_setting);//往菜单内添加QAction
//        popMenu->addAction(action_start_stop_All);
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

    if(currentItem->text().contains(language->device)&&!currentItem->text().contains(language->deviceList))
    {
        ui->Button_import->hide();
        ui->Button_start->hide();
        ui->Button_plot->hide();
        QMenu *popMenu =new QMenu(this);//定义一个右键弹出菜单
        popMenu->addAction(action_device_setting);//往菜单内添加QAction
        popMenu->addAction(action_device_getHistoryData);
        popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
    }

}

void Widget::port_setting_changed(QSerialPort* Port)
{
    this->port = Port;
}

void Widget::device_setting_changed(QString s)
{
    portAgent->Set_Settings(s);
    emit orders(ORDER_CHANGE_SETTINGS,get_device_id());
}

void Widget::port_setting_Dialog_Show()
{
    portSettingDialog->show();
    this->portAgent->isDataRecived = true;
}

void Widget::device_setting_Dialog_Show()
{
    deviceSettingDialog->setDeviceID(get_device_id());
    emit orders(ORDER_GET_SETTINGS,get_device_id());
    deviceSettingDialog->show();
}

void Widget::plot_dialog_show()
{
    plotDialog->plotShow();
}

int Widget::get_device_id()
{
    QStandardItem *currentItem = get_current_item();
    int index = 0;
    QString s = currentItem->text();
    if(currentItem->text().contains(language->device)&&!currentItem->text().contains(language->deviceList));
    else
        s = currentItem->parent()->text();
    if(s == language->historyData|| s == language->realTimeData)
        s = currentItem->parent()->parent()->text();
    index = s.split(" ").at(1).toInt();
    return index;
}

QString Widget::get_device_id_toString()
{
    QStandardItem *currentItem = get_current_item();
    QString s = currentItem->text();
    if(currentItem->text().contains(language->device)&&!currentItem->text().contains(language->deviceList));
    else
        s = currentItem->parent()->text();
    if(s == language->historyData|| s == language->realTimeData)
        s = currentItem->parent()->parent()->text();
   return s.split(" ").at(1);
}

void Widget::start_and_stop_collecting()
{
    if(map.value(get_device_id_toString())==0)
    {
        ui->Button_start->setText(language->buttonStop);
        map.insert(QString::number(get_device_id()),1);
        ui->Button_import->setEnabled(false);
        get_current_item()->setText(language->sampling);
        emit orders(ORDER_START_COLLECTING,get_device_id());
    }
    else
    {
        map.insert(QString::number(get_device_id()),0);
        ui->Button_start->setText(language->buttonStart);
        ui->Button_import->setEnabled(true);
        get_current_item()->setText(language->realTimeData);
        emit orders(ORDER_STOP_COLLECTING,get_device_id());
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
    portSettingDialog->show();
}

void Widget::get_devices_list()
{
    portAgent->setPort(port);
    emit orders(ORDER_GET_DEVICE_LIST,0);
}

void Widget::export_to_excel()
{
    QStringList datalist = getTableData();
//    for(int i=0;i<ui->tableWidget->rowCount()-1;i++)
//    {
//        QString s="";
//        for(int j=0;j<3;j++)
//        {
//            if(j==2)
//                s += ui->tableWidget->item(i,j)->text();
//            else
//                s += (ui->tableWidget->item(i,j)->text()+",");
//        }
//        datalist.append(s);
//    }
    qDebug()<<datalist.at(0);  
    QString fileName = QFileDialog::getSaveFileName(this,"Sava File",get_device_id_toString()+QDateTime::currentDateTime().toString("yyMMddhhmmss"),"Microsoft Office (*.csv)");//获取保存路径
    emit saveAsCsv(datalist,fileName);
}

void Widget::set_progressBar_value(double i)
{
    if((int)(i*100)<100)
    {
        ui->label->setText(language->exporting);
        ui->progressBar->show();
        ui->progressBar->setValue((int)(i*100));
    }
    else
    {
        ui->progressBar->hide();
        ui->label->setText("");
    }
}

bool Widget::isInstanceDataCollecting()
{
    for (QMap<QString, int>::const_iterator it = map_point->cbegin(), end = map_point->cend(); it != end; ++it) {
        if(it.value()==1)
        {
            return true;
        }
     }
    return false;
}

void Widget::setDeviceId(QString id){
    qDebug()<<"id:"<<id;
}

void Widget::getHistoryData(){
    emit orders(ORDER_GET_DEVICE_LIST,get_device_id());
}


























/*
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
*/
