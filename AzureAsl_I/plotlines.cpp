#include "plotlines.h"

PlotLines::PlotLines(t)
{
    setTitle("图的标题");
    //--------设置画布---------//
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas ->setPalette(Qt::white);
    canvas ->setBorderRadius(10);
    setCanvas(canvas);
    plotLayout()->setAlignCanvasToScales(true);

    //-----------设置x，y坐标和范围--------------//
    setAxisTitle( QwtPlot::yLeft, "ylabel" );
    setAxisTitle( QwtPlot::xBottom, "xlabel" );
    setAxisScale(QwtPlot::yLeft,0.0,10.0);
    setAxisScale(QwtPlot::xBottom,0.0,10.0);

    //-----------设置栅格线------------------- //
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::gray, 0 ,Qt::DotLine);
    grid->attach(this);

}

void PlotLines::InitPlot(const QString N_title ,float xmin, float xmax, float ymin, float ymax)
{
    QFont font("Comic Sans MS",9);
    setFont(font);
    setTitle(N_title);

    setAxisScale(QwtPlot::yLeft, ymin, ymax);
    setAxisScale(QwtPlot::xBottom,xmin , xmax);
    curve =new QwtPlotCurve(N_title);
}


void PlotLines::Plot(QPolygonF N_line)
{




    curve->setPen(Qt::blue,1);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setSamples(N_line);
    curve->attach(this);
    curve->setLegendAttribute(curve->LegendShowLine);



}

//点击图例，显示相应的曲线
void PlotLines::showItem(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *plotItem = infoToItem( itemInfo );
    if ( plotItem )
        plotItem->setVisible( on );
}
