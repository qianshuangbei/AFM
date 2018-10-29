#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "qcustomplot.h"
#include "manager/datamanager.h"
#include "surfacegraph.h"


class QPushButton;

namespace Ui {
class MainWindow;
enum Rp_Analysis{
     RpSection               =0x001,
     RpRoughness             =0x002,
     RpPowerSpecralDensity   =0x003
};

}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    double **N_matrix;     //当前数组
    QVector<double**> allChannel ;

    int xOffset ;
    int yOffset ;
    int mSize;     // 行数
    int nSize;     // 列数

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
    void on_actionDouble_Channel_triggered();
    void on_actionThree_Dimension_triggered();
    void on_actionViewer_triggered();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent*);
    void mouseMove(QMouseEvent*);


private:
    void viewerMode();

    QString _fileName;
    Ui::MainWindow *ui;
    QPointF _pos00;
    QPointF _posmn;
    DataManager *_dataManager;
    QLabel *_mouseStartLabel;
    QLabel *_mouseEndLabel;
    QLabel *_mouseMoveLabel;
    QCPColorMap* _doubleColorMap;
    QVector<QVector<int>> _pastSize;
    QCPColorScale *colorScale;
    bool _pressRelease ;
    bool _mouseActive;
    bool _isDoubleChannel;
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

