#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "rowparm.h"
#include "qcustomplot.h"


class QPushButton;

namespace Ui {
class MainWindow;
enum Rp_Analysis{
     RpSection               =0x001,
     RpRoughness             =0x002,
     RpPowerSpecralDensity  =0x003
};

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    RowParm rowInfoZ;
    double **N_matrix;     //当前数组
    QString p_filename;
    QVector<double**> ALL_mat ;

    int sectionxoffset;
    int sectionyoffset;
    int sectionxlength;
    int sectionylength;
    int xoffset ;
    int yoffset ;
    int m_size;
    int n_size;

    QPoint lastPoint;
    QPoint endPoint;
    QPoint curPoint;
public:
    void GenerateInformation();
    bool LoadDataFile(const QString &fileName, const int Ti, double ** matrix);
    void SectionAddData(int x1, int y1, int x2, int y2);
    void InitAfterFile();
    void GenerateMain2DImage();

private slots:
    void on_actionOpen_triggered();
    void on_actionSection_triggered();
    void on_actionRoughness_triggered();
    void on_actionPower_Spectral_Denstiy_triggered();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



    void on_actionDouble_Channel_triggered();

signals:
    void mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent*);
    void mouseMove(QMouseEvent*);


private:
    Ui::MainWindow *ui;
    QPixmap tempPix;  //辅助画布

    QWidget *widget ;

    QPushButton *ChannelButton;
    QPushButton *excute;

    QMouseEvent *Leftclick;

    QPushButton * T_crossbtn ;

    QLabel *mouseStartLabel;
    QLabel *mouseEndLabel;
    QLabel *mouseMoveLabel;

    MainWindow *new_window;

    QTableWidget *newtable;

    QFrame *CLine;
    QFrame *LBox;

    QCustomPlot *R_customPlot;
//    QCustomPlot *L_customPlot;
    QCustomPlot *Double_customPlot;
    QCustomPlot *Double_RcustomPlot;

    QCPColorMap *colorMap;
    QCPColorMap* DoubleColorMap;



    bool PRESS_RELEASE ;
    bool W_NEW;
    bool MouseActive;
    bool isDoubleChannel;
protected:
    void paintEvent(QPaintEvent *);



};

inline void FunctionSwitchBuffer(Ui::Rp_Analysis side){
    switch(side){
        case Ui::RpSection: ;
        case Ui::RpRoughness: ;
        case Ui::RpPowerSpecralDensity: ;
    default: break;
    }
}

#endif // MAINWINDOW_H

