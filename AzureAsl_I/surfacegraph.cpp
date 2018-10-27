#include "surfacegraph.h"

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;

SurfaceGraph::SurfaceGraph(Q3DSurface *surface, double** matrix, int xSize, int ySize)
    : m_graph(surface),
      m_XSize(xSize),
      m_YSize(ySize),
      m_minValue(0.0),
      m_maxValue(0.0)
{
    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_Proxy = new QSurfaceDataProxy();
    m_Series = new QSurface3DSeries(m_Proxy);
    fillProxy(matrix);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::fillProxy(double** matrix)
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(m_XSize);
    for (int i = 0 ; i < m_XSize; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(m_YSize);
        int index = 0;
        for (int j = 0; j < m_YSize; j++) {
            m_minValue = std::min(m_minValue, (float)matrix[i][j]);
            m_maxValue = std::max((float)matrix[i][j], m_maxValue);
            (*newRow)[index++].setPosition(QVector3D(i, matrix[i][j], j));
        }
        *dataArray << newRow;
    }
    m_Proxy->resetArray(dataArray);
}

void SurfaceGraph::enable3DModel(bool enable)
{
    if (enable) {
        m_Series->setDrawMode(QSurface3DSeries::DrawSurface);
        m_Series->setFlatShadingEnabled(false);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(-30, m_XSize+30);
        m_graph->axisY()->setAutoAdjustRange(true);
        m_graph->axisZ()->setRange(-30, m_YSize+30);

        m_graph->addSeries(m_Series);

        m_axisMinSliderX->setMaximum(m_XSize - 2);
        m_axisMinSliderX->setValue(-10);
        m_axisMaxSliderX->setMaximum(m_XSize - 1);
        m_axisMaxSliderX->setValue(m_XSize - 1);
        m_axisMinSliderZ->setMaximum(m_YSize - 2);
        m_axisMinSliderZ->setValue(-10);
        m_axisMaxSliderZ->setMaximum(m_YSize - 1);
        m_axisMaxSliderZ->setValue(m_YSize - 1);
    }
}
void SurfaceGraph::adjustXMin(int min)
{
    float minX = min;

    int max = m_axisMaxSliderX->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderX->setValue(max);
    }
    float maxX = max;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustXMax(int max)
{
    float maxX = max;

    int min = m_axisMinSliderX->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = min;

    setAxisXRange(minX, maxX);
}

void SurfaceGraph::adjustZMin(int min)
{
    float minZ = min;

    int max = m_axisMaxSliderZ->value();
    if (min >= max) {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = max;

    setAxisZRange(minZ, maxZ);
}

void SurfaceGraph::adjustZMax(int max)
{
    float maxX = max;

    int min = m_axisMinSliderZ->value();
    if (max <= min) {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = min;

    setAxisZRange(minX, maxX);
}

//! [5]
void SurfaceGraph::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);
}

void SurfaceGraph::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
}
void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
