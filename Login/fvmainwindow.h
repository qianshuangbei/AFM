#ifndef FVMAINWINDOW_H
#define FVMAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class FVMainWindow;
}

class FVMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FVMainWindow(QWidget *parent = 0);
    ~FVMainWindow();
    bool loadrowFile(const QString &fileName);
    bool loaddatFile(const QString &fileName);
    bool loadFVFile (const QString &fileName );
//    void showcurve ();

    //------------- parameter--------------//
    QVector<double> T_dataoffset;
    QVector<int > T_datalength;
    QVector<QString> T_filelist;
    QVector<double> T_k;
    double T_defsen;    
    double T_TMD;
    double T_sscale;


    double **N_matrix;
    double **FV_trace ;
    double **FV_retrc;
    int FV_n;
    int m_size;
    int n_size;
    double D_max;
    double D_min;
    QCustomPlot *S_cust;




    //------------- parameter--------------//

private slots:
    void on_Address_selectionChanged();

    void on_LoadButton_clicked();

    void on_Meshbtn_clicked();

    void on_StatisticButton_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_YScale_editingFinished();

    void on_Scaleup_editingFinished();

    void on_ScaleDown_editingFinished();

private:
    Ui::FVMainWindow *ui;
    QString curFile;
    QPixmap tempPix;    //辅助画布
    QPoint lastPoint;   //上一次的鼠标位置
    QCustomPlot *customPlot;
 //   QCustomPlot *S_cust;
 //   QWidget *widget ;
    const int pixsize;        //画布大小



  //------------------标志位----------------------------//
     bool enDrawing;     //是否存在画图请求

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

};

#endif // FVMAINWINDOW_H
