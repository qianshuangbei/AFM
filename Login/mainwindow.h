#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loadrowFile(const QString &fileName);
    bool loaddatFile(const QString &fileName);
    void showcurve ();
    void keyPressEvent(QKeyEvent *event);


    //------------- parameter--------------//

    QVector<double> T_dataoffset;
    QVector<int > T_datalength;
    double T_defsen;
    QVector<double> T_k;
    double T_TMD;
    double T_zscale;
    double T_zsen;
    double N_matrix[2][512];
    double Z_size;
    QVector<QString> T_filelist;
    QStringList badlist;
    QStringList goodlist;
    QVector<double> gooddata;
    QVector<double> DMTdata;

    /*

        N_matrix[j] =new float [n];

*/

    //------------- parameter--------------//

private slots:


    void on_LoadButton_clicked();

    void on_Address_selectionChanged();

    void on_listView_clicked(const QModelIndex &index);
//    void on_good_clicked();
//    void on_bad_clicked();

    void on_ScreenButton_clicked();

    void on_DeleteButton_clicked();

    void on_StatisticButton_clicked();

    void on_actionSelect_S_triggered();

    void on_ScaleDown_editingFinished();

    void on_Scaleup_editingFinished();

    void on_YScale_editingFinished();

private:

    Ui::MainWindow *ui;
  //  bool isUntitled;
    QString curFile;
    QCustomPlot *customPlot;
    QCustomPlot *S_cust;
    bool screeningflag;
    bool bgflag;
    QModelIndex curIndex;
};

#endif // MAINWINDOW_H
