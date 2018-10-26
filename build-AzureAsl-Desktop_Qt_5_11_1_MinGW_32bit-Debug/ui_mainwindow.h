/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSection;
    QAction *actionRoughness;
    QAction *actionPower_Spectral_Denstiy;
    QAction *actionDouble_Channel;
    QAction *actionFlatten;
    QAction *actionThree_Dimension;
    QWidget *centralWidget;
    QCustomPlot *CurveImagePlot;
    QCustomPlot *CurveImageIIPlot;
    QCustomPlot *TwoDImageIIPlot;
    QCustomPlot *TwoDImagePlot;
    QTableWidget *newtable;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAnalysis;
    QMenu *menuFilters;
    QMenu *menuCommands;
    QMenu *menuOptions;
    QMenu *menuHistory;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1440, 900);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(11);
        MainWindow->setFont(font);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QStringLiteral(""));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/image/image_open.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionSection = new QAction(MainWindow);
        actionSection->setObjectName(QStringLiteral("actionSection"));
        actionSection->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/image/general.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionSection->setIcon(icon1);
        actionSection->setVisible(true);
        actionRoughness = new QAction(MainWindow);
        actionRoughness->setObjectName(QStringLiteral("actionRoughness"));
        actionPower_Spectral_Denstiy = new QAction(MainWindow);
        actionPower_Spectral_Denstiy->setObjectName(QStringLiteral("actionPower_Spectral_Denstiy"));
        actionDouble_Channel = new QAction(MainWindow);
        actionDouble_Channel->setObjectName(QStringLiteral("actionDouble_Channel"));
        actionFlatten = new QAction(MainWindow);
        actionFlatten->setObjectName(QStringLiteral("actionFlatten"));
        actionThree_Dimension = new QAction(MainWindow);
        actionThree_Dimension->setObjectName(QStringLiteral("actionThree_Dimension"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CurveImagePlot = new QCustomPlot(centralWidget);
        CurveImagePlot->setObjectName(QStringLiteral("CurveImagePlot"));
        CurveImagePlot->setGeometry(QRect(700, 10, 481, 571));
        CurveImageIIPlot = new QCustomPlot(centralWidget);
        CurveImageIIPlot->setObjectName(QStringLiteral("CurveImageIIPlot"));
        CurveImageIIPlot->setGeometry(QRect(700, 590, 481, 41));
        TwoDImageIIPlot = new QCustomPlot(centralWidget);
        TwoDImageIIPlot->setObjectName(QStringLiteral("TwoDImageIIPlot"));
        TwoDImageIIPlot->setGeometry(QRect(700, 650, 491, 51));
        TwoDImagePlot = new QCustomPlot(centralWidget);
        TwoDImagePlot->setObjectName(QStringLiteral("TwoDImagePlot"));
        TwoDImagePlot->setGeometry(QRect(11, 1, 649, 407));
        newtable = new QTableWidget(centralWidget);
        if (newtable->columnCount() < 4)
            newtable->setColumnCount(4);
        if (newtable->rowCount() < 5)
            newtable->setRowCount(5);
        newtable->setObjectName(QStringLiteral("newtable"));
        newtable->setGeometry(QRect(11, 628, 641, 192));
        newtable->setRowCount(5);
        newtable->setColumnCount(4);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1440, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAnalysis = new QMenu(menuBar);
        menuAnalysis->setObjectName(QStringLiteral("menuAnalysis"));
        menuFilters = new QMenu(menuBar);
        menuFilters->setObjectName(QStringLiteral("menuFilters"));
        menuCommands = new QMenu(menuBar);
        menuCommands->setObjectName(QStringLiteral("menuCommands"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuHistory = new QMenu(menuBar);
        menuHistory->setObjectName(QStringLiteral("menuHistory"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAnalysis->menuAction());
        menuBar->addAction(menuFilters->menuAction());
        menuBar->addAction(menuCommands->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuHistory->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuAnalysis->addAction(actionSection);
        menuAnalysis->addAction(actionRoughness);
        menuAnalysis->addAction(actionPower_Spectral_Denstiy);
        menuAnalysis->addAction(actionFlatten);
        menuAnalysis->addAction(actionThree_Dimension);
        menuCommands->addAction(actionDouble_Channel);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSection);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
        actionSection->setText(QApplication::translate("MainWindow", "Section", nullptr));
        actionRoughness->setText(QApplication::translate("MainWindow", "Roughness", nullptr));
        actionPower_Spectral_Denstiy->setText(QApplication::translate("MainWindow", "Power Spectral Denstiy", nullptr));
        actionDouble_Channel->setText(QApplication::translate("MainWindow", "Double Channel", nullptr));
        actionFlatten->setText(QApplication::translate("MainWindow", "Flatten", nullptr));
        actionThree_Dimension->setText(QApplication::translate("MainWindow", "Three Dimension", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuAnalysis->setTitle(QApplication::translate("MainWindow", "Analysis", nullptr));
        menuFilters->setTitle(QApplication::translate("MainWindow", "Filters", nullptr));
        menuCommands->setTitle(QApplication::translate("MainWindow", "Commands", nullptr));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", nullptr));
        menuHistory->setTitle(QApplication::translate("MainWindow", "History", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
