#include "mainwindow.h"

#include <QApplication>
#include "logindialog.h"
#include "fvmainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    MainWindow w;
    FVMainWindow fvw;
    LoginDialog dlg;

    if(dlg.exec() == QDialog::Accepted)
    {

        int T = dlg.L_connect;
        if(T==0){
            w.show();
            return a.exec();
        }
        else if(T==1)
        {
            fvw.show();
            return a.exec();
        }
        else return 0;


    }
    else return 0 ;

     return a.exec();
}
