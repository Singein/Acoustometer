#include "plot.h"
#include "ui_plot.h"


Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
    ui->pushButton->setMaximumHeight(30);
    plot_s = ui->widget_s;
    plot_f = ui->widget_f;
    graph_s = plot_s->addGraph();
    graph_f = plot_f->addGraph();
    viewInit(graph_s,plot_s,STRENGTH);
    viewInit(graph_f,plot_f,FREQUENCY);
    startTime = QDateTime::currentDateTime().toTime_t();
    endTime  = startTime + 30;
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(flexable()));
}

Plot::~Plot()
{
    delete ui;
}

void Plot::viewInit(QCPGraph *graph,QCustomPlot *plot,int type)
{
//    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    if(type == 0){
        plot->yAxis->setLabel("声强(W/cm²)");
        plot->yAxis->setRange(0,250);
        graph->setPen(QPen(QColor(255, 0, 120), 2));
    }
    else{
        plot->yAxis->setLabel("频率(KHz)");
        plot->yAxis->setRange(0,200);
        graph->setPen(QPen(QColor(180, 0, 255), 2));
    }

    plot->addLayer("abovemain", plot->layer("main"), QCustomPlot::limAbove);
    plot->addLayer("belowmain", plot->layer("main"), QCustomPlot::limBelow);
    graph->setLayer("abovemain");
    plot->xAxis->grid()->setLayer("belowmain");
    plot->yAxis->grid()->setLayer("belowmain");
    plot->xAxis->setBasePen(QPen(QColor(200, 200, 200), 1));
    plot->yAxis->setBasePen(QPen(QColor(200, 200, 200), 1));
    plot->xAxis->setTickPen(QPen(Qt::white, 1));
    plot->yAxis->setTickPen(QPen(Qt::white, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridVisible(true);
    plot->yAxis->grid()->setSubGridVisible(true);
    plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    plot->xAxis->setTicker(dateTicker);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    plot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    plot->axisRect()->setBackground(axisRectGradient);
    plot->rescaleAxes();
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plot::addNodes(QStringList data,QString title)
{
    this->setWindowTitle(title);
    dataCount = data.length();
    QVector<QCPGraphData> stren(dataCount);
    QVector<QCPGraphData> frequ(dataCount);
    if(!data.isEmpty())
    {
        double now = QDateTime::fromString(data.at(0).split(",").at(0),"yyyy-MM-dd hh:mm:ss").toTime_t();
        endTime = QDateTime::fromString(data.at(dataCount-1).split(",").at(0),"yyyy-MM-dd hh:mm:ss").toTime_t();
        startTime = now;
        plot_s->xAxis->setRange(startTime, endTime);
        plot_s->yAxis->setRange(0, 250);
        plot_f->xAxis->setRange(startTime, endTime);
        plot_f->yAxis->setRange(0, 200);
        for(int i=0;i<dataCount;i++)
        {
            stren[i].key = QDateTime::fromString(data.at(i).split(",").at(0),"yyyy-MM-dd hh:mm:ss").toTime_t();
            frequ[i].key = QDateTime::fromString(data.at(i).split(",").at(0),"yyyy-MM-dd hh:mm:ss").toTime_t();
            stren[i].value = data.at(i).split(",").at(1).toDouble();
            frequ[i].value = data.at(i).split(",").at(2).toDouble();
        }
    }
    graph_s->data()->set(stren);
    graph_f->data()->set(frequ);
    plot_s->replot();
    plot_f->replot();
}

void Plot::flexable()
{
    plot_s->xAxis->setRange(startTime, endTime);
    plot_s->yAxis->setRange(0, 250);
    plot_f->xAxis->setRange(startTime, endTime);
    plot_f->yAxis->setRange(0, 200);
    plot_s->replot();
    plot_f->replot();
}

void Plot::plotShow()
{
    plot_s->xAxis->setRange(startTime, endTime);
    plot_f->xAxis->setRange(startTime, endTime);
    this->show();
}
