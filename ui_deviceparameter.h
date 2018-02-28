/********************************************************************************
** Form generated from reading UI file 'deviceparameter.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEPARAMETER_H
#define UI_DEVICEPARAMETER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceParameter
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_k;
    QSpinBox *spinBox_K;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_H;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox_G;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBox_F;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBox_T;
    QSpinBox *spinBox_T;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *checkBox_OFF;
    QSpinBox *spinBox_OFF;
    QLabel *label_6;
    QFrame *line;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_11;
    QSpinBox *spinBox_instance;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QLabel *label_time;
    QLabel *label_7;
    QLabel *label_time_device;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QLabel *label_ID;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *buttonApplay;
    QPushButton *buttonCancel;

    void setupUi(QWidget *DeviceParameter)
    {
        if (DeviceParameter->objectName().isEmpty())
            DeviceParameter->setObjectName(QStringLiteral("DeviceParameter"));
        DeviceParameter->resize(634, 348);
        DeviceParameter->setStyleSheet(QStringLiteral(""));
        verticalLayout_4 = new QVBoxLayout(DeviceParameter);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        groupBox = new QGroupBox(DeviceParameter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(180, 16777215));
        groupBox->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_k = new QLabel(groupBox);
        label_k->setObjectName(QStringLiteral("label_k"));

        horizontalLayout->addWidget(label_k);

        spinBox_K = new QSpinBox(groupBox);
        spinBox_K->setObjectName(QStringLiteral("spinBox_K"));

        horizontalLayout->addWidget(spinBox_K);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        doubleSpinBox_H = new QDoubleSpinBox(groupBox);
        doubleSpinBox_H->setObjectName(QStringLiteral("doubleSpinBox_H"));

        horizontalLayout_2->addWidget(doubleSpinBox_H);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer = new QSpacerItem(153, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        comboBox_G = new QComboBox(groupBox);
        comboBox_G->setObjectName(QStringLiteral("comboBox_G"));

        horizontalLayout_3->addWidget(comboBox_G);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalSpacer_3 = new QSpacerItem(153, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_3);

        horizontalSpacer_4 = new QSpacerItem(153, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_4);

        verticalSpacer_2 = new QSpacerItem(20, 67, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_8->addWidget(groupBox);

        groupBox_3 = new QGroupBox(DeviceParameter);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        checkBox_F = new QCheckBox(groupBox_3);
        checkBox_F->setObjectName(QStringLiteral("checkBox_F"));

        verticalLayout_2->addWidget(checkBox_F);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        checkBox_T = new QCheckBox(groupBox_3);
        checkBox_T->setObjectName(QStringLiteral("checkBox_T"));

        horizontalLayout_7->addWidget(checkBox_T);

        spinBox_T = new QSpinBox(groupBox_3);
        spinBox_T->setObjectName(QStringLiteral("spinBox_T"));

        horizontalLayout_7->addWidget(spinBox_T);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_7->addWidget(label_4);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        checkBox_OFF = new QCheckBox(groupBox_3);
        checkBox_OFF->setObjectName(QStringLiteral("checkBox_OFF"));

        horizontalLayout_9->addWidget(checkBox_OFF);

        spinBox_OFF = new QSpinBox(groupBox_3);
        spinBox_OFF->setObjectName(QStringLiteral("spinBox_OFF"));

        horizontalLayout_9->addWidget(spinBox_OFF);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_9->addWidget(label_6);


        verticalLayout_2->addLayout(horizontalLayout_9);


        verticalLayout_3->addLayout(verticalLayout_2);

        line = new QFrame(groupBox_3);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout_3->addWidget(label_9);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        spinBox_instance = new QSpinBox(groupBox_3);
        spinBox_instance->setObjectName(QStringLiteral("spinBox_instance"));

        horizontalLayout_11->addWidget(spinBox_instance);


        verticalLayout_3->addLayout(horizontalLayout_11);

        verticalSpacer_3 = new QSpacerItem(20, 127, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        horizontalLayout_8->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(DeviceParameter);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        verticalLayout_5 = new QVBoxLayout(groupBox_2);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_5->addWidget(label_5);

        label_time = new QLabel(groupBox_2);
        label_time->setObjectName(QStringLiteral("label_time"));

        verticalLayout_5->addWidget(label_time);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_5->addWidget(label_7);

        label_time_device = new QLabel(groupBox_2);
        label_time_device->setObjectName(QStringLiteral("label_time_device"));

        verticalLayout_5->addWidget(label_time_device);

        verticalSpacer = new QSpacerItem(20, 130, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        horizontalLayout_8->addWidget(groupBox_2);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(DeviceParameter);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_10->addWidget(label_8);

        label_ID = new QLabel(DeviceParameter);
        label_ID->setObjectName(QStringLiteral("label_ID"));
        label_ID->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_10->addWidget(label_ID);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_5);

        buttonApplay = new QPushButton(DeviceParameter);
        buttonApplay->setObjectName(QStringLiteral("buttonApplay"));
        buttonApplay->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_10->addWidget(buttonApplay);

        buttonCancel = new QPushButton(DeviceParameter);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));
        buttonCancel->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_10->addWidget(buttonCancel);


        verticalLayout_4->addLayout(horizontalLayout_10);


        retranslateUi(DeviceParameter);

        QMetaObject::connectSlotsByName(DeviceParameter);
    } // setupUi

    void retranslateUi(QWidget *DeviceParameter)
    {
        DeviceParameter->setWindowTitle(QApplication::translate("DeviceParameter", "Form", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("DeviceParameter", "\351\207\207\346\240\267\345\217\202\346\225\260", Q_NULLPTR));
        label_k->setText(QApplication::translate("DeviceParameter", "K", Q_NULLPTR));
        label_2->setText(QApplication::translate("DeviceParameter", "H\357\274\210W/cm\302\262)", Q_NULLPTR));
        label_3->setText(QApplication::translate("DeviceParameter", "G", Q_NULLPTR));
        comboBox_G->clear();
        comboBox_G->insertItems(0, QStringList()
         << QApplication::translate("DeviceParameter", "High", Q_NULLPTR)
         << QApplication::translate("DeviceParameter", "Medium", Q_NULLPTR)
         << QApplication::translate("DeviceParameter", "Low", Q_NULLPTR)
         << QApplication::translate("DeviceParameter", "Auto", Q_NULLPTR)
        );
        groupBox_3->setTitle(QApplication::translate("DeviceParameter", "\344\273\252\345\231\250\350\256\276\347\275\256", Q_NULLPTR));
        checkBox_F->setText(QApplication::translate("DeviceParameter", "F", Q_NULLPTR));
        checkBox_T->setText(QApplication::translate("DeviceParameter", "T", Q_NULLPTR));
        label_4->setText(QApplication::translate("DeviceParameter", "s", Q_NULLPTR));
        checkBox_OFF->setText(QApplication::translate("DeviceParameter", "OFF", Q_NULLPTR));
        label_6->setText(QApplication::translate("DeviceParameter", "min", Q_NULLPTR));
        label_9->setText(QApplication::translate("DeviceParameter", "\345\256\236\346\227\266\346\225\260\346\215\256\351\207\207\351\233\206\351\227\264\351\232\224 s", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("DeviceParameter", "\346\227\266\351\227\264\346\240\241\346\255\243", Q_NULLPTR));
        label_5->setText(QApplication::translate("DeviceParameter", "\345\275\223\345\211\215\346\227\266\351\227\264", Q_NULLPTR));
        label_time->setText(QApplication::translate("DeviceParameter", "2016-12-15 12:30:21", Q_NULLPTR));
        label_7->setText(QApplication::translate("DeviceParameter", "\344\273\252\345\231\250\346\227\266\351\227\264", Q_NULLPTR));
        label_time_device->setText(QApplication::translate("DeviceParameter", "2016-12-15 12:30:21", Q_NULLPTR));
        label_8->setText(QApplication::translate("DeviceParameter", "\345\275\223\345\211\215\351\200\211\344\270\255\344\273\252\345\231\250\357\274\232", Q_NULLPTR));
        label_ID->setText(QApplication::translate("DeviceParameter", "\345\243\260\345\274\272\344\273\252#001", Q_NULLPTR));
        buttonApplay->setText(QApplication::translate("DeviceParameter", "\346\233\264\346\224\271\345\271\266\345\272\224\347\224\250", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("DeviceParameter", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DeviceParameter: public Ui_DeviceParameter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEPARAMETER_H
