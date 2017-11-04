#include "loginui.h"
#include "ui_loginui.h"

LoginUI::LoginUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginUI)
{

    ui->setupUi(this);
    ui->Loginbtn->setStyleSheet("background-color: rgb(0,0,0,0);");

    xtime = startTimer(30);
    timestamp = 0;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(unsigned int i=0; i<CODESIZE; i++) {
        Codeparam *codevc = new Codeparam(1,1,1,1);
        codevc->Code_Toum = qrand()%128+127;
        codevc->Code_speed = qrand()%4+1;
        codevc->Code_size =qrand()%30+6;
        codevc->Code_Yval =qrand()%1200;
        codevc->Code_position = qrand()%1440;
        codevc->Code =QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2);
        codevct.push_back(codevc);
    }


}
void LoginUI::init_code(Codeparam* codevc){
    codevc->Code_Toum = qrand()%128+127;
    codevc->Code_speed = qrand()%4+1;
    codevc->Code_size =qrand()%30+6;
    codevc->Code_Yval =qrand()%1200;
    codevc->Code_position = qrand()%800;
    codevc->Code = QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2);
}

LoginUI::~LoginUI()
{
    delete ui;
}

void LoginUI::on_Loginbtn_clicked()
{
    accept();
}

void LoginUI::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    painter = new QPainter(this);
    painter->setPen(QColor(69,139,0));
    QFont font("Microsoft YaHei", 12);
    painter->setFont(font);
    QTransform transform;
    transform.rotate(+270.0);//旋转90度
    painter->setWorldTransform(transform);
    for(unsigned int i=0; i<CODESIZE; i++){
        font.setPointSize(codevct[i]->Code_size);
        painter->setPen(QColor(69,139,codevct[i]->Code_Toum,codevct[i]->Code_Toum));
        painter->setFont(font);
        codevct[i]->Code_Yval-=codevct[i]->Code_speed;
        painter->drawText(codevct[i]->Code_Yval,codevct[i]->Code_position, codevct[i]->Code);
    }


}
void LoginUI::timerEvent(QTimerEvent *event){
    if(timestamp++> 50) accept();;
    if(event->timerId()== xtime){
        repaint();
    }


}
