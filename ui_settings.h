/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QComboBox *comboBox_CheckBit;
    QComboBox *comboBox_StopBit;
    QComboBox *comboBox_Port;
    QComboBox *comboBox_DataBit;
    QComboBox *comboBox_Baud;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_5;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ScanButton;
    QPushButton *OpenButton;
    QPushButton *Button_OK;

    void setupUi(QWidget *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QStringLiteral("Settings"));
        Settings->resize(308, 238);
        Settings->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        verticalLayout = new QVBoxLayout(Settings);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(Settings);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        comboBox_CheckBit = new QComboBox(Settings);
        comboBox_CheckBit->setObjectName(QStringLiteral("comboBox_CheckBit"));

        gridLayout->addWidget(comboBox_CheckBit, 4, 1, 1, 1);

        comboBox_StopBit = new QComboBox(Settings);
        comboBox_StopBit->setObjectName(QStringLiteral("comboBox_StopBit"));

        gridLayout->addWidget(comboBox_StopBit, 3, 1, 1, 1);

        comboBox_Port = new QComboBox(Settings);
        comboBox_Port->setObjectName(QStringLiteral("comboBox_Port"));

        gridLayout->addWidget(comboBox_Port, 0, 1, 1, 1);

        comboBox_DataBit = new QComboBox(Settings);
        comboBox_DataBit->setObjectName(QStringLiteral("comboBox_DataBit"));

        gridLayout->addWidget(comboBox_DataBit, 2, 1, 1, 1);

        comboBox_Baud = new QComboBox(Settings);
        comboBox_Baud->setObjectName(QStringLiteral("comboBox_Baud"));

        gridLayout->addWidget(comboBox_Baud, 1, 1, 1, 1);

        label_2 = new QLabel(Settings);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_4 = new QLabel(Settings);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label = new QLabel(Settings);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(Settings);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(17, 37, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ScanButton = new QPushButton(Settings);
        ScanButton->setObjectName(QStringLiteral("ScanButton"));

        horizontalLayout->addWidget(ScanButton);

        OpenButton = new QPushButton(Settings);
        OpenButton->setObjectName(QStringLiteral("OpenButton"));

        horizontalLayout->addWidget(OpenButton);

        Button_OK = new QPushButton(Settings);
        Button_OK->setObjectName(QStringLiteral("Button_OK"));

        horizontalLayout->addWidget(Button_OK);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Settings);

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QWidget *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Form", Q_NULLPTR));
        label_3->setText(QApplication::translate("Settings", "\346\225\260\346\215\256\344\275\215", Q_NULLPTR));
        comboBox_CheckBit->clear();
        comboBox_CheckBit->insertItems(0, QStringList()
         << QApplication::translate("Settings", "None", Q_NULLPTR)
         << QApplication::translate("Settings", "Odd", Q_NULLPTR)
         << QApplication::translate("Settings", "Even", Q_NULLPTR)
         << QApplication::translate("Settings", "Space", Q_NULLPTR)
         << QApplication::translate("Settings", "Mark", Q_NULLPTR)
        );
        comboBox_StopBit->clear();
        comboBox_StopBit->insertItems(0, QStringList()
         << QApplication::translate("Settings", "1", Q_NULLPTR)
         << QApplication::translate("Settings", "1.5", Q_NULLPTR)
         << QApplication::translate("Settings", "2", Q_NULLPTR)
        );
        comboBox_DataBit->clear();
        comboBox_DataBit->insertItems(0, QStringList()
         << QApplication::translate("Settings", "5", Q_NULLPTR)
         << QApplication::translate("Settings", "6", Q_NULLPTR)
         << QApplication::translate("Settings", "7", Q_NULLPTR)
         << QApplication::translate("Settings", "8", Q_NULLPTR)
        );
        comboBox_Baud->clear();
        comboBox_Baud->insertItems(0, QStringList()
         << QApplication::translate("Settings", "1200", Q_NULLPTR)
         << QApplication::translate("Settings", "2400", Q_NULLPTR)
         << QApplication::translate("Settings", "4800", Q_NULLPTR)
         << QApplication::translate("Settings", "9600", Q_NULLPTR)
         << QApplication::translate("Settings", "19200", Q_NULLPTR)
         << QApplication::translate("Settings", "38400", Q_NULLPTR)
         << QApplication::translate("Settings", "57600", Q_NULLPTR)
         << QApplication::translate("Settings", "115200", Q_NULLPTR)
        );
        label_2->setText(QApplication::translate("Settings", "\346\263\242\347\211\271\347\216\207", Q_NULLPTR));
        label_4->setText(QApplication::translate("Settings", "\345\201\234\346\255\242\344\275\215", Q_NULLPTR));
        label->setText(QApplication::translate("Settings", "\344\270\262\345\217\243\345\217\267", Q_NULLPTR));
        label_5->setText(QApplication::translate("Settings", "\346\240\241\351\252\214\344\275\215", Q_NULLPTR));
        ScanButton->setText(QApplication::translate("Settings", "\346\211\253\346\217\217\344\270\262\345\217\243", Q_NULLPTR));
        OpenButton->setText(QApplication::translate("Settings", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        Button_OK->setText(QApplication::translate("Settings", "\347\241\256\345\256\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
