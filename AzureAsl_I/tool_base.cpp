#include "tool_base.h"
#include <algorithm>

ToolBase::ToolBase()
{
    imageXOffset=0;
    imageYOffset=0;
    imageXLength=0;
    imageYLength=0;
    xSize=1;
    ySize=1;
    xScale=1.0;
    yScale=1.0;
}


QVector<QVector<double> > ToolBase::ToolCrossLine(double **matrix, int lastX, int lastY, int endX, int endY)
{

    QVector<QVector<double> > res(2);
    int x1 = (lastX-imageXOffset)/(double)imageXLength* xSize;
    int y1 = ((imageYOffset-lastY))/(double)imageYLength*ySize;
    int x2 = (endX-imageXOffset)/(double)imageXLength* xSize;
    int y2 = ((imageYOffset-endY))/(double)imageYLength*ySize;


    for(int p = qMin(x1, x2); p < qMin(x1, x2) + qAbs(x1 - x2); p++)
    {
        int tmpLine;
        tmpLine = (y2-y1)/(double)(x2-x1)*(p-x1)+y1;
        if(tmpLine>=0&&p>=0 &&tmpLine<ySize && p<xSize){
            res[0].push_back(p);
            res[1].push_back( matrix[tmpLine][p]);
        }
    }
    return res;
}


QVector<QVector<double> >ToolBase::ToolHPowerSpectralDensity(double **matrix){
    int N = xSize;
    QVector<QVector<double> > res(2, QVector<double>(xSize/2,0));
    double *in  = (double*) fftw_malloc(sizeof(double)*N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    for(int i=0; i<N/2; i++){
        double axe= 0.5*xSize/(xScale)- i/(xScale);
        axe = log(axe);
        res[0][i]=axe;
    }
    for(int j = 0; j<ySize; j++){
        for(int i=0; i<N; i++) in[i]= matrix[j][i];
        fftw_plan p = fftw_plan_dft_r2c_1d(N, in,out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int i=0; i<N/2; i++){
            double value = (xScale/xSize)/xSize*qAbs(out[0][i]*out[0][i]+out[1][i]*out[1][i]);
            value = log(value);
            res[1][xSize/2-1-i] += value;
        }
        fftw_destroy_plan(p);
    }
    for(int i=0; i<xSize/2; i++) res[1][i] /= ySize;
    fftw_free(out);
    return res;

}

QVector<QVector<double> >ToolBase::ToolVPowerSpectralDensity(double **matrix){
    int N = ySize;
    QVector<QVector<double> >res(2, QVector<double>(ySize/2,0));
    double *in = (double*) fftw_malloc(sizeof(double)*N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    for(int i=0; i<N/2; i++){
        double axe= 0.5*ySize/(xScale)- i/(xScale);
        axe = log(axe);
        res[0][i]=axe;
    }
    for(int j = 0; j<xSize; j++){
        for(int i=0; i<N; i++) in[i]= matrix[i][j];
        fftw_plan p = fftw_plan_dft_r2c_1d(N, in,out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int i=0; i<N/2; i++){
            double value = (xScale/xSize)/ySize*qAbs(out[0][i]*out[0][i]+out[1][i]*out[1][i]);
            value = log(value);
            res[1][N/2-1-i] += value;
        }
        fftw_destroy_plan(p);
    }
    for(int i=0; i<ySize/2; i++) res[1][i] /= xSize;
    fftw_free(out);
    return res;
}
double *ToolBase::Historgram(double **matrix, int mSize,int nSize)
{
    int Size = nSize*mSize;
    double HData[Size];
    int tempi = 0;
    for(int ix = 0;ix < mSize; ix++){
        for(int iy = 0;iy < nSize; iy++){
            HData[tempi++] = matrix[ix][iy];
        }
    }
    qSort(HData,HData+Size);
  //  qsort(HData, Size, sizeof(double),compare);//生序排列
    double Dmax = HData[Size-1];
    double Dmin = HData[0];
    double Rx = Dmax - Dmin;//求出极差
    int Kx = 2 + 3.32*log10(Size); //史特吉斯组数经验公式
    double hx = Rx / Kx;//求组距
    double Lu[Kx];
    double Ld[Kx];
    Ld[0] = HData[0];
    for(int i = 0;i < Kx; i++){
        Lu[i] = Ld[i] + hx;
        Ld[i+1] = Lu[i];
    }//求出各组上下限
    int fnum = 0;//次数统计
    int knum = 0;//组号
    double Ksum[Kx];//组数记录
    int ik = 0;
    for(int ix = 0; ix < Size; ix++){
        if(HData[ix] < Lu[knum]) fnum++;
        else{
            Ksum[ik++] = fnum;
            fnum = 1;
            knum++;
        }
    }
    double *fresum = new double[2*Kx+1];
    for(int ix = 1;ix <= Kx;ix++) fresum[ix+Kx] = Ksum[ix-1]  / 65536 * 100;
    fresum[0] = Kx;
    for(int ix = 1;ix <= Kx;ix++) fresum[ix] = Lu[ix-1];
    QFile openDefaultDir("HistogramData.txt");
    QTextStream in(&openDefaultDir);
    openDefaultDir.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    knum = 1;
    for(int ix = 1;ix <= Kx;ix++) in <<"The frequency of data's up-limited "<< Lu[ix-1]<<": "<< fresum[ix+Kx]  <<endl;
    openDefaultDir.close();
    return fresum;//fresum[0]为组数 从[1]-[Kx]开始为频率信息 [Kx+1]-[2*Kx+1]

}
