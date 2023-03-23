#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

void linear_regression(int num, float x[], float y[], int ilow, int ihigh, float *a, float *err_a, float *b, float *err_b, float *r)
{
  /* Computes the linear best fit to data - y = a*x + b, where x and y are 
     arrays of size num. The values used in the arrays are those from ilow 
     to ihigh. (ie. if the full arrays are being used, then ilow=0 and 
     ihigh=num-1.
     Returns the values of a & b (with errors) 
     as well as r, the regression coefficient. 
  */

  float sumx,sumy,sumxx,sumxy,sumyy;
  int i,count;

  *a=0.;
  *b=0.;
  *r=0.;

  if (ilow>ihigh) {
    printf("Bollocks! linear_regression.c :: ilow (%d) > ihigh (%d)!!\n",ilow,ihigh);
    exit(1);
  }
  if (ihigh>num-1) {
    printf("Bollocks! linear_regression.c :: ihigh (%d) out of bounds of array!!\n",ihigh);
    exit(1);
  }
  if(ilow<0){
    printf("Bollocks! linear_regression.c :: ilow (%d) < 0. !!\n",ilow);
    exit(1);
  }

  sumx=0.;
  sumy=0.;
  sumxx=0.;
  sumxy=0.;
  sumyy=0.;
  count=0;
  for (i=ilow;i<=ihigh;i++){
    count++;
    sumx = sumx + x[i];
    sumy = sumy + y[i];
    sumxx = sumxx + x[i]*x[i];
    sumxy = sumxy + x[i]*y[i];
    sumyy = sumyy + y[i]*y[i];
    //  printf("num = %d,  ilow = %d, ihigh = %d, x[%d] = %1.2e, sum = %1.2e y[%d] = %1.2e sum = %1.2e count = %d \n", num, ilow, ihigh, i, x[i], sumx, i, y[i], sumy, count);
  }
  if(count!=(ihigh-ilow+1)){
    printf("Whoops!! count (%d) doesn't equal what it should (%d)!!\n",count,(ihigh-ilow+1));
    exit(1);
  }

  *a = (count*sumxy - sumx*sumy)/(count*sumxx - sumx*sumx);
  *err_a = count / (count*sumxx - sumx*sumx);
  *b = (sumy*sumxx - sumxy*sumx)/(count*sumxx - sumx*sumx);
  *err_b = sumxx / (count*sumxx - sumx*sumx);

  *r = (count*sumxy - sumx*sumy)/(sqrt(count*sumxx-sumx*sumx)*sqrt(count*sumyy-sumy*sumy));
}
