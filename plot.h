#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <qcustomplot.h>
#include <QVector>
#include <lang.h>

#define STRENGTH 0
#define FREQUENCY 1

namespace Ui {
class Plot;
}


class Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();
    void plotShow();
    void setLanguage(LANG *language);

private:
    Ui::Plot *ui;
    QList<double> f;
    QList<double> s;
    QCustomPlot *plot;
    QCPGraph *graph_s;
    QCPGraph *graph_f;
    double startTime;
    double endTime;
    int dataCount;
    void graphInit(QCPGraph *graph,QCustomPlot *plot,int type);
    void viewInit();
    LANG *language;



private slots:
    void addNodes(QStringList data,QString title);
    void flexable();
};

#endif // PLOT_H
