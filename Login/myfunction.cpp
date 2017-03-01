#include "myfunction.h"


using namespace std;

double DMTmodel(double *y_matrix, const double radiu, const double possion, const int y_length, const double pixel)
{
  if(radiu==0||possion>1||possion<=0||y_length<1) return 0;


  QVector<double> DMT_E;
  int polybegin= 0.4*y_length;
  int polyend  = 0.9*y_length;
  double avg = 0.000;             // begin position
  for(int i=polybegin; i<polyend; i++)
  {
      avg+= y_matrix[i];
  }
  avg = avg/(polyend-polybegin);

  int p=0;
  while(y_matrix[p]>avg) p++;
  int t=0;
  while(y_matrix[t]>avg)
  {
      double dt=(p-t+1)*pixel;
      double curE = 0.7500*(1-possion*possion)/sqrt(radiu)/sqrt(dt*dt*dt)* (y_matrix[t]-avg);
      DMT_E.push_back(curE);
      t++;
  }
  double Eavg=0.000;
  for(int i=0; i<DMT_E.size();i++)
  {
      Eavg +=DMT_E[i];
  }
  return Eavg/DMT_E.size();


}
