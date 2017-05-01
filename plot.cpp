#include "plot.h"
#include "ui_plot.h"


Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
    f = new QList<double>;
    s = new QList<double>;
    plot_s = ui->widget_s;
    plot_f = ui->widget_f;
    graph_s = plot_s->addGraph();
    graph_f = plot_f->addGraph();
    viewInit(graph_s,plot_s);
    viewInit(graph_f,plot_f);
}

Plot::~Plot()
{
    delete ui;
}

void Plot::setAgent(PortAgent *agent)
{
    this->Agent = agent;
    connect(this->Agent,SIGNAL(addPlotNode(double,double)),this,SLOT(addNodes(double,double)));
}

void Plot::viewInit(QCPGraph *graph,QCustomPlot *plot)
{
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph->setPen(QPen(QColor(120, 120, 120), 2));
    plot->addLayer("abovemain", plot->layer("main"), QCustomPlot::limAbove);
    plot->addLayer("belowmain", plot->layer("main"), QCustomPlot::limBelow);
    graph->setLayer("abovemain");
    plot->xAxis->grid()->setLayer("belowmain");
    plot->yAxis->grid()->setLayer("belowmain");
    plot->xAxis->setBasePen(QPen(Qt::white, 1));
    plot->yAxis->setBasePen(QPen(Qt::white, 1));
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
    plot->yAxis->setRange(0, 50);
}

void Plot::addNodes(double x, double y)
{
    s->append(x);
    f->append(y);
    int length = s->length();
    QVector<double> x1(length);
    QVector<double> y1(length);
    QVector<double> y2(length);

    for(int i=0;i<length;i++)
    {
        x1[i] = i;
        y1[i] = f->at(i)+i*2;
        y2[i] = 2*i*i;
    }

    graph_s->setData(x1, y1);
    graph_f->setData(x1, y2);

    plot_s->replot();
    plot_f->replot();


}
