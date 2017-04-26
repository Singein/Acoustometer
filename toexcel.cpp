#include "toexcel.h"
#include "ui_toexcel.h"
#include <QDebug>

Excel::Excel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToExcel)
{
    ui->setupUi(this);
    this->setWindowTitle("正在导出到Excel");
    setWindowModality(Qt::ApplicationModal);
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->label->setMaximumHeight(30);
    ui->label->setMinimumHeight(30);
    this->setMaximumSize(300,100);
    this->setMinimumSize(300,100);
    connect(this,SIGNAL(current_progress(double)),this,SLOT(set_progressBar_value(double)));
    connect(ui->progressBar,SIGNAL(valueChanged(int)),this,SLOT(progressBar_finished(int)));
    connect(this,SIGNAL(show_time()),this,SLOT(time_to_show()));
}

Excel::~Excel()
{
    delete ui;
}

void Excel::setCsv(Csv *csv)
{
    this->csv = csv;
}

void Excel::Export(QString filename, QStringList data)
{
    qDebug()<<"import called";
    QString fileName = QFileDialog::getSaveFileName(this,"选择保存的路径",filename,"Microsoft Office (*.csv)");//获取保存路径

    if (fileName.isEmpty()) {
//        QMessageBox::critical(0, tr("错误"), tr("要保存的文件名为空！"));
        return;
    }
    emit show_time();
    QStringList header;
    header<<"时间"<<"声强"<<"频率";
    for(int i=0;i<=data.length();i++)
    {
        if(i==0)
            csv->addRow(header,filename);
        csv->addRow(data.at(i-1).split(","),filename);
        emit current_progress(((double)i)/((double)data.length()));
    }


//    QAxObject *excel = new QAxObject("Excel.Application");
//    excel->dynamicCall("SetVisible(bool)", false); //如果为了看自己的程序到底怎样工作，可以设置为true

//    excel->setProperty("Visible", false);
//    QAxObject *workbooks = excel->querySubObject("WorkBooks");
//    workbooks->dynamicCall("Add");
//    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
//    QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

//    for(int i=1;i<=data.length()+1;i++)
//    {
//        for(int j=1;j<=3;j++)
//        {
//            QAxObject *range = worksheet->querySubObject("Cells(int,int)", i,j);//row  column为要插入的行和列
//            if(i==1)
//            {
//                if(j==1)
//                    range->setProperty("Value", "时间"); //此处写要插入的内容
//                if(j==2)
//                    range->setProperty("Value", "声强");
//                if(j==3)
//                    range->setProperty("Value", "频率");
//            }
//            else
//                range->setProperty("Value", data.at(i-2).split(",").at(j-1));
//        }
//        emit current_progress(((double)i)/((double)data.length()));
//    }
////    QAxObject *range = worksheet->querySubObject("Cells(int,int)", 1,1);//row  column为要插入的行和列
////    range->setProperty("Value", "12324324"); //此处写要插入的内容
//    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName));
//    workbook->dynamicCall("Close()");
//    worksheet->clear();//释放所有工作表
//    excel->dynamicCall("Quit()");
//    delete excel;//释放excel
}

void Excel::set_progressBar_value(double i)
{
    ui->progressBar->setValue((int)(i*100));
    qDebug()<<ui->progressBar->value();
}

void Excel::progressBar_finished(int i)
{
    if(i == 100)
        this->close();
}

void Excel::time_to_show()
{
    this->show();
}
