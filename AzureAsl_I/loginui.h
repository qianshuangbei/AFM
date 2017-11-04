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
    struct Codeparam{
        QString Code;
        int Code_Toum;
        int Code_Yval;
        int Code_speed;
        int Code_size;
        int Code_position;
        Codeparam(int y,int sp, int si, int p):Code_Yval(y),Code_speed(sp), Code_size(si), Code_position(p){}

    };

public:
    explicit LoginUI(QWidget *parent = 0);
    void paintEvent(QPaintEvent*event);
    void timerEvent(QTimerEvent *event);
    void init_code(Codeparam*codevc);
    ~LoginUI();

private slots:
    void on_Loginbtn_clicked();

private:
    Ui::LoginUI *ui;
    QPainter *painter;
    QVector<Codeparam*> codevct;
    int xtime;
    int timestamp;
    const static unsigned int CODESIZE= 100;

};

#endif // LOGINUI_H
