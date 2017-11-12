#-------------------------------------------------
#
# Project created by QtCreator 2016-10-10T14:32:25
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = AzureAsl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rowparm.cpp \
    tool_base.cpp \
    qcustomplot.cpp \
    loginui.cpp \
    utils.cpp \
    connect/fileconnect.cpp \
    manager/datamanager.cpp \
    manager/nanomodule.cpp \
    connect/filemodule.cpp

HEADERS  += mainwindow.h \
    rowparm.h \
    tool_base.h \
    qcustomplot.h \
    loginui.h \
    fftw3.h \
    utils.h \
    connect/fileconnect.h \
    connect/filemodule.h \
    manager/datamanager.h \
    manager/nanomodule.h

FORMS    += mainwindow.ui \
    loginui.ui

RC_ICONS =desk.ico\

LIBS += E:\Qt\5.5\mingw492_32\fftw-3.3.4-dll32\libfftw3-3.dll
INCLUDEPATH += E:\Qt\5.5\mingw492_32\fftw-3.3.4-dll32
DEPENDPATH += E:\Qt\5.5\mingw492_32\fftw-3.3.4-dll32




RESOURCES += \
    src.qrc
CONFIG += c++11
