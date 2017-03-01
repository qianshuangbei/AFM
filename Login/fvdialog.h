#ifndef FVDIALOG_H
#define FVDIALOG_H

#include <QDialog>

namespace Ui {
class FVDialog;
}

class FVDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FVDialog(QWidget *parent = 0);
    ~FVDialog();

private:
    Ui::FVDialog *ui;
};

#endif // FVDIALOG_H
