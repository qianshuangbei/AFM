#include "tool_base.h"


Tool_base::Tool_base()
{
    imagexoffset=0;
    imageyoffset=0;
    imagexlength=0;
    imageylength=0;
    x_size=1;
    y_size=1;
    x_scale=1.0;
    y_scale=1.0;
}


QVector<QVector<double> > Tool_base::Tool_CrossLine(double **N_matrix, int LastPx, int LastPy, int Endx, int Endy)
{

    QVector<QVector<double> > res(2);
    int x1 = (LastPx-imagexoffset)/(double)imagexlength* x_size;
    int y1 = ((imageyoffset-LastPy))/(double)imageylength*y_size;
    int x2 = (Endx-imagexoffset)/(double)imagexlength* x_size;
    int y2 = ((imageyoffset-Endy))/(double)imageylength*y_size;


    for(int p = qMin(x1,x2); p<qMin(x1,x2)+qAbs(x1-x2) ; p++)
    {
        int Nu_line;
        Nu_line = (y2-y1)/(double)(x2-x1)*(p-x1)+y1;
        if(Nu_line>=0&&p>=0 &&Nu_line<y_size && p<x_size){
            res[0].push_back(p);
            res[1].push_back( N_matrix[Nu_line][p]);
        }
    }
    return res;

}


QVector<QVector<double> >Tool_base::Tool_HPowerSpectralDensity(double **N_matrix){
    int N = x_size;
    QVector<QVector<double> > res(2, QVector<double>(x_size/2,0));
    double *in  = (double*) fftw_malloc(sizeof(double)*N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    for(int i=0; i<N/2; i++){
        double axe= 0.5*x_size/(x_scale)- i/(x_scale);
        axe = log(axe);
        res[0][i]=axe;
    }
    for(int j = 0; j<y_size; j++){
        for(int i=0; i<N; i++) in[i]= N_matrix[j][i];
        fftw_plan p = fftw_plan_dft_r2c_1d(N, in,out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int i=0; i<N/2; i++){
            double value = (x_scale/x_size)/x_size*qAbs(out[0][i]*out[0][i]+out[1][i]*out[1][i]);
            value = log(value);
            res[1][x_size/2-1-i] += value;
        }
        fftw_destroy_plan(p);
    }
    for(int i=0; i<x_size/2; i++) res[1][i] /= y_size;
    fftw_free(out);

    return res;

}

QVector<QVector<double> >Tool_base::Tool_VPowerSpectralDensity(double **N_matrix){
    int N = y_size;
    QVector<QVector<double> >res(2, QVector<double>(y_size/2,0));
    double *in = (double*) fftw_malloc(sizeof(double)*N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    for(int i=0; i<N/2; i++){
        double axe= 0.5*y_size/(x_scale)- i/(x_scale);
        axe = log(axe);
        res[0][i]=axe;
    }
    for(int j = 0; j<x_size; j++){
        for(int i=0; i<N; i++) in[i]= N_matrix[i][j];
        fftw_plan p = fftw_plan_dft_r2c_1d(N, in,out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int i=0; i<N/2; i++){
            double value = (x_scale/x_size)/y_size*qAbs(out[0][i]*out[0][i]+out[1][i]*out[1][i]);
            value = log(value);
            res[1][N/2-1-i] += value;
        }
        fftw_destroy_plan(p);
    }
    for(int i=0; i<y_size/2; i++) res[1][i] /= x_size;
    fftw_free(out);
    return res;
}
