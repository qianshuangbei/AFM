#ifndef LOGINUI_H
#define LOGINUI_H
#include <QPainter>
#include <QDialog>
#include <QtGlobal>
#include <QVector>
#include <QTime>

namespace Ui {
class LoginUI;
}

class LoginUI : public QDialog
{
    Q_OBJECT
public:
/*
 * Parameters to adjust pattern;
*/
    struct codeParam{
        QString code;
        int codeToum;
        int codeYval;
        int codeSpeed;
        int codeSize;
        int codePosition;
        codeParam(int y,int sp, int si, int p):codeYval(y),codeSpeed(sp), codeSize(si), codePosition(p){}

    };

public:
    explicit LoginUI(QWidget *parent = 0);
    void paintEvent(QPaintEvent*event);
    void timerEvent(QTimerEvent *event);
    void init_code(codeParam*codevc);

    ~LoginUI();
private slots:
    void on_Loginbtn_clicked();

private:
    Ui::LoginUI *ui;

    QVector<codeParam*> _codevct;
    int _xTime;
    int _timeStamp;
    const static unsigned int _CODESIZE= 100;

};

#endif // LOGINUI_H
