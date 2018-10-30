#include "loginui.h"
#include "ui_loginui.h"

LoginUI::LoginUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginUI)
{

    ui->setupUi(this);
    ui->Loginbtn->setStyleSheet("background-color: rgb(0,0,0,0);");

    _xTime = startTimer(30);
    _timeStamp = 0;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(unsigned int i=0; i<_CODESIZE; i++) {
        codeParam *codevc = new codeParam(1,1,1,1);
        codevc->codeToum = qrand()%128+127;
        codevc->codeSpeed = qrand()%4+1;
        codevc->codeSize =qrand()%30+6;
        codevc->codeYval =qrand()%1200;
        codevc->codePosition = qrand()%1440;
        codevc->code =QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2);
        _codevct.push_back(codevc);
    }


}
void LoginUI::init_code(codeParam* codevc){
    codevc->codeToum = qrand()%128+127;
    codevc->codeSpeed = qrand()%4+1;
    codevc->codeSize =qrand()%30+6;
    codevc->codeYval =qrand()%1200;
    codevc->codePosition = qrand()%800;
    codevc->code = QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2)+QString::number(qrand()%107,2);
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
    QPainter *_painter = new QPainter(this);
    _painter->setPen(QColor(69,139,0));
    QFont font("Microsoft YaHei", 12);
    _painter->setFont(font);
    QTransform transform;
    transform.rotate(+270.0);//旋转90度
    _painter->setWorldTransform(transform);
    for(unsigned int i=0; i<_CODESIZE; i++){
        font.setPointSize(_codevct[i]->codeSize);
        _painter->setPen(QColor(69,139,_codevct[i]->codeToum,_codevct[i]->codeToum));
        _painter->setFont(font);
        _codevct[i]->codeYval-=_codevct[i]->codeSpeed;
        _painter->drawText(_codevct[i]->codeYval,_codevct[i]->codePosition, _codevct[i]->code);
    }
}
void LoginUI::timerEvent(QTimerEvent *event){
    if(_timeStamp++> 50) accept();
    if(event->timerId()== _xTime){
        repaint();
    }
}
