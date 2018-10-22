#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "qcustomplot.h"
#include "manager/datamanager.h"


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

//    double **N_matrix;     //当前数组
    QString p_filename;
    QVector<double**> ALL_mat ;

    int sectionxoffset;
    int sectionyoffset;
    int sectionxlength;
    int sectionylength;
    int xoffset ;
    int yoffset ;
    int m_size;     // 行数
    int n_size;     // 列数

    QPoint lastPoint;
    QPoint endPoint;
    QPoint curPoint;
public:
    void GenerateInformation();
    bool LoadDataFile(const QString &fileName, const int index, double ** matrix);
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

    QPointF pos_00;
    QPointF pos_mn;
    DataManager *_datamanager;
    QLabel *mouseStartLabel;
    QLabel *mouseEndLabel;
    QLabel *mouseMoveLabel;

 //   QCPColorMap *colorMap;
    QCPColorMap* DoubleColorMap;

    bool PRESS_RELEASE ;
    bool MouseActive;
    bool isDoubleChannel;
    QVector<QVector<int>> PAST_SIZE;
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

