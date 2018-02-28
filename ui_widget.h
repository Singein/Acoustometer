/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_2;
    QProgressBar *progressBar;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QPushButton *Button_plot;
    QPushButton *Button_start;
    QPushButton *Button_import;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(818, 482);
        Widget->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeView = new QTreeView(Widget);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setContextMenuPolicy(Qt::CustomContextMenu);
        treeView->setAcceptDrops(false);
        treeView->setStyleSheet(QStringLiteral(""));
        treeView->setLineWidth(0);
        treeView->setHeaderHidden(true);

        horizontalLayout->addWidget(treeView);

        tableWidget = new QTableWidget(Widget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout->addWidget(tableWidget);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        horizontalLayout_2->addWidget(progressBar);

        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalSpacer = new QSpacerItem(30, 27, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(verticalSpacer);

        Button_plot = new QPushButton(Widget);
        Button_plot->setObjectName(QStringLiteral("Button_plot"));

        horizontalLayout_2->addWidget(Button_plot);

        Button_start = new QPushButton(Widget);
        Button_start->setObjectName(QStringLiteral("Button_start"));
        Button_start->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_2->addWidget(Button_start);

        Button_import = new QPushButton(Widget);
        Button_import->setObjectName(QStringLiteral("Button_import"));
        Button_import->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_2->addWidget(Button_import);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        label->setText(QString());
        Button_plot->setText(QApplication::translate("Widget", "\344\272\214\347\273\264\346\233\262\347\272\277", Q_NULLPTR));
        Button_start->setText(QApplication::translate("Widget", "\345\274\200\345\247\213\351\207\207\351\233\206", Q_NULLPTR));
        Button_import->setText(QApplication::translate("Widget", "\345\257\274\345\207\272\346\225\260\346\215\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
