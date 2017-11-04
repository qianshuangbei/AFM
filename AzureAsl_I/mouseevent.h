#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>

class MouseEvent : public QMainWindow
{
    Q_OBJECT
public:
    explicit MouseEvent(QWidget *parent = 0);
    ~MouseEvent();

protected:
    void mousePressEvent(QMouseEvent *Leftclick);
 //   void mouseMoveEvent(QMouseEvent *e);

signals:

public slots:

private:

};

#endif // MOUSEEVENT_H
