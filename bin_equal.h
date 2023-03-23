#include "/Users/stephencurran/C/functions/mean_func.h"
#include "/Users/stephencurran/C/functions/mean_func2.h"

void bin_equal(float z_i,float z_f,float err_x[],float err_y[],float X[],float Y[],float up[],float dn[],float left[],float right[],int Z,int Zcount,int numChannels,int bin,int flag_y,char outfile[],FILE *f3,FILE *f1,FILE *f2,FILE *f4,int k,int i,float x[],float y[],int n,int N,int N2,float VAL[],float *val,float V,float *mean,float prob,float E,float e[],float expec,float v[],float var,float med,float ave,float sigma,float VAL2[], float *val2,float V2,float *mean2,float prob2,float E2,float e2[],float expec2,float v2[],float var2,float ave2,float sigma2,char systemCall[],FILE *fsize,char sigma_string[],int span_x,char SDerr[], int censor){

  //system("sort -n -k 1 dets.co > all_z.as");
  //sprintf(systemCall,"wc -l all_z.as > /tmp/filesize"); system(systemCall);
  //fsize = fopen("/tmp/filesize","r"); fscanf(fsize,"%d",&numChannels); fclose(fsize); 
  system("rm bin_equal.dat");

  // printf("\n numChannels = %d bin = %d, Zcount = %d\n", numChannels,bin, Zcount);
  for (Z = 0; Z < Zcount ; Z++){ 
    X[Z] = Y[Z] = err_y[Z] = left[Z] = right[Z] = dn[Z] = up[Z] = 0;

     system("rm Z_curr.as");
   
     f3 = fopen("all_z.as","r");for (i = 0; fscanf(f3,"%f %f", &x[i], &y[i]) !=EOF; i++) {
       k = bin+(Z*bin);
       if (i >= Z*bin && i < k){
	 z_i = x[Z*bin]; z_f = x[k-1]; // was k-1 but didn't work for 69/SED_comp.c
	 f1 = fopen("mean_out.txt", "a");fprintf(f1, "%1.3f\n", x[i]); fclose(f1);
	 f2 = fopen("mean_out2.txt", "a");fprintf(f2, "%1.3f\n", y[i]); fclose(f2);
	 // printf("Z = %d, i = %d, k = %d, bin = %d,  z_i = %1.3f, x[%d] = %1.4f z_f = %1.3f\n", Z,i,  k, bin, z_i, i, x[i], z_f);
	 //system("cp mean_out.txt x_test.dat");
	 //sprintf(systemCall,"cp mean_out2.txt Z_%d.as", Z); system(systemCall);
      }
     } fclose(f3);
     

     if (span_x ==1){
     X[Z] = z_i + (z_f - z_i)/2; left[Z] =  z_i; right[Z] = z_f; err_x[Z] = X[Z] - left[Z];
      }
     else{
       mean_func(numChannels, i, systemCall, fsize, mean, VAL, val, V, f1, prob, E, e,expec,v,var,sigma_string, med);
       f3= fopen("mean_values.txt","r");while (fscanf(f3,"%f %f %d", &ave, &sigma, &n) !=EOF);
       X[Z] =  ave; fclose(f3);  // always close otherswise leak memory!!
       if (strcmp(SDerr,"s") == 0) err_x[Z] = sigma;
       else err_x[Z] = sigma/pow(numChannels,0.5); 
       left[Z] = X[Z] - err_x[Z]; right[Z] = X[Z] + err_x[Z];
     }
    

       mean_func2(numChannels, i, systemCall, fsize, mean2, VAL2, val2, V2, f1, prob2, E2, e2, expec2,v2,var2,sigma_string, med);
     f4= fopen("mean_values2.txt","r");while (fscanf(f4,"%f %f %d", &ave2, &sigma2, &n) !=EOF)
					; fclose(f4);
     Y[Z] =  ave2; 
     if (strcmp(SDerr,"s") == 0) err_y[Z] = sigma2; 
     else err_y[Z] = sigma2/pow(numChannels,0.5); 
     dn[Z] = Y[Z] - err_y[Z]; up[Z] = Y[Z] + err_y[Z];
     
   	////////////////////////////////////////
     //	 printf("Z = %d Zcount = %d, z[%d] = %1.3f +/- %1.3f Y[%d] = %1.3f +/- %1.3f\n",  Z, Zcount, Z, X[Z], err_x[Z],Z, Y[Z], err_y[Z]);

	 f4 = fopen("bin_equal.dat", "a");  fprintf(f4, "%1.3e %1.3e %1.3e %1.3e\n", X[Z],err_x[Z], Y[Z], err_y[Z]); fclose(f4); //in case needed
	 
  }
}
