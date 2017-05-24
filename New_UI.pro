#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T21:38:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = New_UI
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    portagent.cpp \
    settings.cpp \
    deviceparameter.cpp \
    crccheck.cpp \
    csv.cpp \
    datasaver.cpp \
    excel.cpp \
    qcustomplot.cpp \
    plot.cpp \
    lang.cpp

HEADERS  += widget.h \
    portagent.h \
    settings.h \
    deviceparameter.h \
    crccheck.h \
    csv.h \
    datasaver.h \
    excel.h \
    qcustomplot.h \
    plot.h \
    lang.h

FORMS    += widget.ui \
    settings.ui \
    deviceparameter.ui \
    plot.ui
QT += serialport
QT += sql
QT += axcontainer
QT += widgets printsupport

RESOURCES += \
    style.qrc

RC_ICONS = myapp.ico
TRANSLATIONS = app.ts
