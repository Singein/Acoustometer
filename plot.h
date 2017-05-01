#ifndef PLOT_H
#define PLOT_H

#include <QDialog>

namespace Ui {
class Plot;
}

class Plot : public QDialog
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();

private:
    Ui::Plot *ui;
};

#endif // PLOT_H
