#ifndef TOEXCEL_H
#define TOEXCEL_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <csv.h>

namespace Ui {
class ToExcel;
}

class Excel : public QWidget
{
    Q_OBJECT

public:
    explicit Excel(QWidget *parent = 0);
    ~Excel();
    void Export(QString filename,QStringList data);
    void setCsv(Csv *csv);

signals:
    void current_progress(double progress);
    void show_time();

private:
    Ui::ToExcel *ui;
    Csv *csv;

private slots:
    void set_progressBar_value(double i);
    void progressBar_finished(int i);
    void time_to_show();
};

#endif // TOEXCEL_H
