#ifndef PLOTLINES_H
#define PLOTLINES_H
#include "qcustomplot.h"

class PlotLines
{
    Q_OBJECT
public:
    explicit PlotLines();

    void Plot(QPolygonF N_line);
    void InitPlot(const QString N_title, float xmin ,float xmax , float ymin, float ymax);
 //   virtual void clearall();
 //   virtual void clearone();



private Q_SLOTS:
    void showItem(const QVariant &itemInfo, bool on);//点击图例，显示相应的曲线

private:
 //   QwtPlotCurve *curve ;
};

#endif // PLOTLINES_H
