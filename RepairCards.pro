#-------------------------------------------------
#
# Project created by QtCreator 2019-02-24T16:28:15
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RepairCards
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    databaseconnector.cpp \
    addform.cpp \
    mainform.cpp \
    addpresenter.cpp \
    mainpresenter.cpp \
    helper.cpp \
    edithandbookform.cpp

HEADERS  += \
    databaseconnector.h \
    handbook.h \
    client.h \
    repaircard.h \
    cardmethod.h \
    addform.h \
    mainform.h \
    addpresenter.h \
    mainpresenter.h \
    iaddview.h \
    imainview.h \
    helper.h \
    edithandbookform.h \
    ihandbookeditview.h \
    iview.h \
    formmode.h

FORMS    += \
    addform.ui \
    mainform.ui \
    edithandbookform.ui
