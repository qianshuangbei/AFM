#include "fvdialog.h"
#include "ui_fvdialog.h"

FVDialog::FVDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FVDialog)
{
    ui->setupUi(this);
}

FVDialog::~FVDialog()
{
    delete ui;
}
