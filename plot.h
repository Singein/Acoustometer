#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <portagent.h>
#include <qcustomplot.h>
#include <QVector>

#define STRENGTH 0
#define FREQUENCY 1

namespace Ui {
class Plot;
}
class PortAgent;
class Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();
    void setAgent(PortAgent *agent);

private:
    Ui::Plot *ui;
    PortAgent *Agent;
    QList<double> f;
    QList<double> s;
    QCustomPlot *plot_s;
    QCustomPlot *plot_f;
    QCPGraph *graph_s;
    QCPGraph *graph_f;
    void viewInit(QCPGraph *graph,QCustomPlot *plot,int type);


private slots:
    void addNodes(QStringList data,QString title);
    void flexable();
};

#endif // PLOT_H
