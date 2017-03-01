#ifndef MYFUNCTION
#define MYFUNCTION

#endif // MYFUNCTION
#include <QVector>
#include "math.h"

template <typename T1 ,typename T2 >
void F_polyfit(const T1 *y_matrix, const T2* x_matrix, const int N_length , double *res)
{
    double sum_x=0;
    double sum_y=0;
    double sum_xy=0;
    double avg_of_x2=0;
    double avg_of_x=0;
    double avg_of_y=0;

    for(int i=0; i<N_length; i++)
    {
        sum_x = sum_x+x_matrix[i];
        sum_y = sum_y+y_matrix[i];
        sum_xy = sum_xy+x_matrix[i]*y_matrix[i];
        avg_of_x2 = avg_of_x2 +x_matrix[i]*x_matrix[i];
    }
    avg_of_x = sum_x/N_length;
    avg_of_y = sum_y/N_length;

    double a =(sum_xy-sum_x*sum_y/N_length)/(avg_of_x2- sum_x*sum_x/N_length);

    double b= avg_of_y - a*avg_of_x;

    res[0] = a;
    res[1] = b;

}

template <typename T1, typename T2>
void F_polyCalibration(T1 *y_matrix, const T2*x_matrix, const int N_length, const double *calkb) //调平校0
{
    for(int i=0 ; i<N_length; i++)
    {
        y_matrix[i] = y_matrix[i]- calkb[0]*x_matrix[i]-calkb[1];
    }

}

template <typename T>
T findmin(T *y_matrix , const int y_length )
{
    if( y_length < 1 ) return 0;

    //y_matrix : pos of matrix begin;
    //y_length : range of search;
    T y_min = y_matrix[0];
    for(int i=1 ; i<y_length; i++)
    {
        y_min = y_min<y_matrix[i]?y_min:y_matrix[i];
    }
    return y_min;
}


template <typename T>
T findmax(T *y_matrix , const int y_length )
{
    if(y_length < 1 ) return 0;

    //y_matrix : pos of matrix begin;
    //y_length : range of search;
    T y_max = y_matrix[0];
    for(int i=1 ; i<y_length; i++)
    {
        y_max = y_max>y_matrix[i]?y_max:y_matrix[i];
    }
    return y_max;
}

double DMTmodel(double *y_matrix, const double radiu, const double possion, const int y_length, const double pixel);
