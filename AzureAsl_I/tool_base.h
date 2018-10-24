#ifndef TOOL_BASE_H
#define TOOL_BASE_H

#include "qcustomplot.h"
#include "fftw3.h"

class ToolBase
{

public:
    int imageXOffset;
    int imageYOffset;
    int imageXLength;
    int imageYLength;
    int xSize;
    int ySize;
    double xScale;
    double yScale;


public:
    explicit ToolBase();
    QVector<QVector<double> > ToolCrossLine(double **matrix, int lastX, int lastY, int endX, int endY);
    QVector<QVector<double> > ToolHPowerSpectralDensity(double **matrix);
    QVector<QVector<double> > ToolVPowerSpectralDensity(double **matrix);
signals:

public slots:

private:

};

#endif // TOOL_BASE_H
