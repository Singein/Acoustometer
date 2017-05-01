#ifndef PLOT_H
#define PLOT_H

#include <QDialog>
#include <portagent.h>

namespace Ui {
class Plot;
}
class PortAgent;
class Plot : public QDialog
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();
    void setAgent(PortAgent *Agent);

private:
    Ui::Plot *ui;

    PortAgent *Agent;

private slots:
    void addNodes(double x,double y);
};

#endif // PLOT_H
