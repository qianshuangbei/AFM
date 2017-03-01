#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T12:38:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Login
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    qcustomplot.cpp \
    fvmainwindow.cpp \
    myfunction.cpp


HEADERS  += mainwindow.h \
    logindialog.h \
    qcustomplot.h \
    fvmainwindow.h \
    myfunction.h



FORMS    += mainwindow.ui \
    logindialog.ui \
    fvmainwindow.ui


RESOURCES += \
    myresource.qrc
