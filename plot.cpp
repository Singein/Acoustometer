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
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(flexable()));
}

Plot::~Plot()
{
    delete ui;
}

void Plot::setAgent(PortAgent *agent)
{
    this->Agent = agent;
//    connect(this->Agent,SIGNAL(addPlotNode(double,double,QString)),this,SLOT(addNode(double,double,QString)));
}

void Plot::viewInit(QCPGraph *graph,QCustomPlot *plot,int type)
{
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    plot->xAxis->setLabel("时间(s)");
    if(type == 0){
        plot->yAxis->setLabel("声强(W/cm²)");
        plot->xAxis->setRange(0, 10);
        plot->yAxis->setRange(0,250);
        graph->setPen(QPen(QColor(255, 0, 120), 2));
    }
    else{
        plot->yAxis->setLabel("频率(KHz)");
        plot->xAxis->setRange(0, 10);
        plot->yAxis->setRange(0,200);
        graph->setPen(QPen(QColor(120, 0, 255), 2));
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
    int length = data.length();
    QVector<double> x1(length);
    QVector<double> y1(length);
    QVector<double> y2(length);
    for(int i=0;i<length;i++)
    {
        x1[i] = i;
        y1[i] = data.at(i).split(",").at(1).toDouble();
        y2[i] = data.at(i).split(",").at(2).toDouble();
    }
    graph_s->setData(x1, y1);
    graph_f->setData(x1, y2);
    plot_s->replot();
    plot_f->replot();
}


void Plot::flexable()
{
    plot_s->xAxis->setRange(0, graph_s->dataCount());
    plot_s->yAxis->setRange(0, 250);

    plot_f->xAxis->setRange(0, graph_f->dataCount());
    plot_f->yAxis->setRange(0, 200);
    plot_s->replot();
    plot_f->replot();
}

