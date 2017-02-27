#ifndef TOEXCEL_H
#define TOEXCEL_H

#include <QWidget>
#include <QDir>
#include <QAxObject>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class ToExcel;
}

class ToExcel : public QWidget
{
    Q_OBJECT

public:
    explicit ToExcel(QWidget *parent = 0);
    ~ToExcel();
    void Import(QString filename,QStringList data);


signals:
    void current_progress(double progress);
    void show_time();

private:
    Ui::ToExcel *ui;

private slots:
    void set_progressBar_value(double i);
    void progressBar_finished(int i);
    void time_to_show();
};

#endif // TOEXCEL_H
