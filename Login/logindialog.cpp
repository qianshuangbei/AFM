#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    L_connect =0;
    QPixmap iconmap;
    iconmap.load("E:\\Qt program\\Login\\logo.jpg");
    ui->iconpic->setPixmap(iconmap);
    QPixmap Sgmap;
    Sgmap.load("E:\\Qt program\\Login\\Sglogo1.jpg");
    ui->SForce->setIcon(Sgmap);
    Sgmap.load("E:\\Qt program\\Login\\FVlogo.png");
    ui->VForce->setIcon(Sgmap);
    Sgmap.load("Alogo.jpg");

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
  //  accept();
    if((ui->userlineEdit->text()==tr("qianshuangbei")&&ui->pwdlineEdit->text()==tr("12020229")) || 1)
    {
    accept();
    }
    else
    {
        QMessageBox::warning (this, tr("warning"),tr("user name or password error!"),QMessageBox::Yes);
        ui->userlineEdit->clear();
        ui->pwdlineEdit->clear();
        ui->userlineEdit->setFocus();
    }
    /*
    if(L_connect==0 )
    {

        this->close();
        mainw1.show();
        this->show();
    }
    else if(L_connect==0)
    {

        this->close();
        fvw1.show();
        this->show();
    }

*/
}

void LoginDialog::on_SForce_clicked()
{
    L_connect = 0;
    ui->textEdit->setText("Single Force Module");

}

void LoginDialog::on_VForce_clicked()
{
   L_connect = 1;
   ui->textEdit->setText("Force Volumn Module");
}
