#include "mainwindow.h"
#include "loginui.h"
#include <QApplication>
#include <QtCore/QFile>
#include <QtCharts>
using namespace QtCharts;
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    LoginUI loginexe;
    QPalette mainwindow_color;

    /*
    QFile qss(":/Qss/white.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();
*/
    mainwindow_color.setColor(QPalette::Background, Qt::black);
    loginexe.setWindowOpacity(1);
    loginexe.setWindowFlags(Qt::FramelessWindowHint);
    loginexe.setPalette(mainwindow_color);

    if(loginexe.exec()==QDialog::Accepted){
        w.showMaximized();
        return a.exec();
    }
    else return 0;

}










