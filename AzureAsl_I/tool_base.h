#ifndef TOOL_BASE_H
#define TOOL_BASE_H

#include "qcustomplot.h"
#include "fftw3.h"

class Tool_base
{

public:
    int imagexoffset;
    int imageyoffset;
    int imagexlength;
    int imageylength;
    int x_size;
    int y_size;
    double x_scale;
    double y_scale;


public:
    explicit Tool_base();
    QVector<QVector<double> > Tool_CrossLine(double **N_matrix, int LastPx, int LastPy, int Endx, int Endy);
    QVector<QVector<double> > Tool_HPowerSpectralDensity(double **N_matrix);
    QVector<QVector<double> > Tool_VPowerSpectralDensity(double **N_matrix);
signals:

public slots:

private:

};

#endif // TOOL_BASE_H
