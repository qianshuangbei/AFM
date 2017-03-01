#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();


private slots:
    void on_loginBtn_clicked();

    void on_SForce_clicked();

    void on_VForce_clicked();

private:
    Ui::LoginDialog *ui;


public:
    int L_connect;   // 0 : Single Force
                     // 1 : FV
                     //

};

#endif // LOGINDIALOG_H
