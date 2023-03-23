#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <cpgplot.h>
#include "/Users/stephencurran/C/functions/prob_print.h"
#include "/Users/stephencurran/C/functions/linear_regression.c"
#include "/Users/stephencurran/C/functions/LBT_ticks2.h" 
#include "/Users/stephencurran/C/functions/grey_scale.h"
#include "/Users/stephencurran/C/functions/bin_equal.h"
//#include "/Users/stephencurran/C/functions/bin_x.h"

// gcc -c pred_z.c -I/usr/local/pgplot -o pred_z.o; gfortran -o pred_z pred_z.o -I/opt/local/include -L/opt/local/lib -lcpgplot -lpgplot -lpng -lz; ./pred_z

int main()
{
  //////////// STANDARD STUFF /////////////////
  int i, j, l, m, p,q, censor, bin, numChannels,no,sym,colour, idet, inon;
  char format[100],infile[300], dir[100],lineInput[99999],ps_out[100], ps_out_dev[100],systemCall[300],label[100],ref[10];
  float x1, x2, y1, y2, x[99999], y[99999],up[9999], dn[9999],left[9999], right[9999], xline[999], yline[999];
  double h = 6.626069e-34;
  FILE *f1,*f2,*f3,*f4,*f5,*f6,*f7,*f8,*f9,*f10,*f11,*f12,*f13,*f15,*f16,*f17,*f18,*f19,*f20,*f21,*f22,*fsize,*fdir;
  ////////////// MEAN ////////////////
  int n, N, N2;
  float a,a_err,b,b_err,r; 
  char sigma_string[200];
  float VAL[99999], *val, V, *mean,  E, e[99999],expec, v[99999], var, med, ave, sigma, VAL2[99999], *val2, V2, *mean2, E2, e2[99999],expec2, v2[99999], var2, ave2, sigma2; 
  long double prob, prob2;
  /////////////// LINEAR REGRESSION AND SURVIVAL ///////////////////
  float intercept,gradient,z_value,A,Bee, Sigma, Prob; 
  float A_all,Bee_all, Sigma_all,  A_N,Bee_N, Sigma_N,Sigma_Q;
  double Prob_all, Prob_N,  Prob_Q;
  char as_name[20];
  ///////// COSMOLOGY ////////////////
  int npts,k; 
  float z, r_e,r_a,temp, ztemp, om0 = 0.27, ol0 = 0.83, age;
  double dz, h0 = 71,drbydz, dtbydz,D_co, D_a, D_e, D_L, d_l;  
  //////////// PLOT SET-UP //////////////
  float text = 1.8, xbottom = -0.2, xtop = 4.5, ybottom = xbottom, ytop =xtop; 
  float height = 0.8, width = 0.83, start = 0.15, startx = 0.12,  panel = 0.35, gap = 0.1;  
  float xstart =  xbottom + (xtop-xbottom)/24, ystart =  ytop-(ytop-ybottom)/16, yskip = (ytop-ybottom)/16, xskip = (xtop-xbottom)/7; 
  /////////// ACTUAL STUFF /////
  float z_spec, z_pred, mmax, Dz, x_1[99999], y_1[99999];
  float u_mag,g_mag,r_mag,i_mag,z_mag,W1_mag,W2_mag,NUV_mag,FUV_mag,count[99999];
  int num;

  system("rm *.as inv_sigma_IN sigma_IN"); system("rm *.co  gauss.dat gauss_norm.dat");
 
  strcpy(infile, "TF-mags.dat");
  // strcpy(infile, "X-Milliquas_photos.dat");
  //strcpy(infile, "X-SDSS_photos.dat");
  //strcpy(infile, "ML_fluxes-imp-SVR_out.dat"); 
  //strcpy(infile, "ML_OHE-SVR_out.dat"); 
  //strcpy(infile, "TF-OHE_n=70.dat");

   bin = 100;

   sprintf(ps_out, "pred_z_%s-%d.eps", infile,bin); sprintf(ps_out_dev, "%s/cps", ps_out);
  printf("Output format, screen [x] or file [p]?"); scanf("%s", format);
  if (strcmp(format, "x") == 0 )  cpgopen("/xs"); 
  else cpgopen(ps_out_dev);
  cpgpap(7.0,0.7);
 
   cpgqvp(0,&x1,&x2,&y1,&y2);
  cpgsch(text);cpgslw(3);
 
  cpgsvp(x1+0.05,x2-0.0,start,start+height);
  cpgswin(xbottom, xtop, ybottom, ytop); 
 
  i = j = 0;
  f1 = fopen(infile,"r"); 
  while (fscanf(f1,"%f%f",&z_spec, &z_pred) !=EOF){
      f2= fopen("data1.co", "a"); fprintf(f2, "%1.4f %1.4f\n", z_spec, z_pred); fclose(f2);
      f2 = fopen("mean1.co", "a");  fprintf(f2, "%1.3f\n", z_spec - z_pred); fclose(f2);
      f2= fopen("norm.co", "a"); fprintf(f2, "%1.5f\n", (z_spec - z_pred)/(z_spec+1)); fclose(f2);
      x_1[j] = z_spec; y_1[j] = z_pred;
      count[j] = z_spec - z_pred;
      
      //cpgsch(0.7*text);cpgsci(1); cpgpt1(z_spec,z_pred,17);
      j++;
   
  }fclose(f1);
  //system("cp mean0.co check.txt");
cpgsch(text);

 /////////// GREYSCALE HAS TO BE UNDERNEATH  ///////////////
  //   xbottom = ybottom = 0;
   int no_y, no_x =80;  // was 50
   float x_size, y_size,px1,px2,py1,py2,NO_Y,kx,ky;
   char shade[10],grey[50],key_corn[10],label_b[40][20];
  
   x_size = (xtop-xbottom)/no_x; y_size = x_size; // set y size to the same just now
   NO_Y = (ytop - ybottom)/y_size;  no_y = (int)NO_Y;

   float NO_XY = no_x*no_y;
   int no_xy = (int)NO_XY, *M,maxNB; //printf("NO_XY = %1.2f no_xy = %d\n", NO_XY,no_xy);
   int pn[no_xy], nb[no_x][no_y], max_nb, log_shade=0, start_shade,stop_shade,span_shade,col[99];; 
   float NB[no_x][no_y], MAX_NB,inc1,inc2,key_x,key_y,key_skip; // positioning of key
  
   key_skip = yskip/1.5; sprintf(key_corn, "br"); // bottom right
   kx = 6; ky = 40; start_shade = 10;stop_shade =0; 

   system("cp data1.co all.co");
   grey_scale(f3,x,y,p,q,i,j,colour,xbottom,xtop,ybottom,ytop,yskip,text,no_y,no_x,x_size,y_size,px1,px2,py1,py2,NO_Y,shade,grey,NO_XY,no_xy,pn,nb,max_nb,log_shade,start_shade,stop_shade,span_shade,col,NB,MAX_NB,inc1,inc2,key_x,key_y,key_skip,key_corn,label_b,kx,ky,M,maxNB);

   //////////////////////////////////////////////////////////////
   linear_regression(j,x_1,y_1,0,j-1,&a,&a_err,&b,&b_err,&r); 
  system("cp mean1.co mean_out.txt"); 
  mean_func(numChannels, i, systemCall, fsize, mean, VAL, val, V, f1, prob, E, e,expec,v,var,sigma_string,med);
  
  f3= fopen("mean_values.txt","r");while (fscanf(f3,"%f %f %d", &ave, &sigma, &n) !=EOF);
  fclose(f3);

  float ave_norm, sigma_norm;
  system("cp norm.co mean_out.txt");
  mean_func(numChannels, i, systemCall, fsize, mean, VAL, val, V, f1, prob, E, e,expec,v,var,sigma_string,med);
  f3= fopen("mean_values.txt","r");while (fscanf(f3,"%f %f %d", &ave_norm, &sigma_norm, &n) !=EOF);
  fclose(f3);

  int jcut = j/2;// integer will round down
  float JCUT = (float)j/2, median,NMAD,MAD;
 system("awk < mean1.co '{if ($1 < 0) $1 = -1*$1; print $0}' | sort -n -k1g > sorted.co");
 f1 = fopen("sorted.co","r"); 
 for (i = 0; fscanf(f1,"%f", &x[i]) !=EOF; i++){
   if (i == jcut){
     if (JCUT == (float)jcut) median = x[i];
     else if (JCUT > (float)jcut) median = (x[i]+x[i-1])/2; // value in between
   }
     }fclose(f1);
 if (median < 1) MAD = 1.48*median; 
 else MAD = 1.48*median;

 system("awk < norm.co '{if ($1 < 0) $1 = -1*$1; print $0}' | sort -n -k1g > norm_sorted.co");
 f1 = fopen("norm_sorted.co","r"); 
 for (i = 0; fscanf(f1,"%f", &x[i]) !=EOF; i++){
   if (i == jcut){
     if (JCUT == (float)jcut) median = x[i];
     else if (JCUT > (float)jcut) median = (x[i]+x[i-1])/2; // value in between
   }
     }fclose(f1);

 if (median < 1) NMAD = 1.48*median; 
 else NMAD = 1.48*median;
 
  /// ALSO WANT OUTLER FRACTIONS - fraction with |\Delta  z(\text{norm})|> 0.15|
 system("awk < norm.co '{if ($1 > 0.15 || $1 < -0.15) print $0}' > outliers.dat");
 sprintf(systemCall,"wc -l outliers.dat > /tmp/filesize"); system(systemCall);
 fsize = fopen("/tmp/filesize","r"); fscanf(fsize,"%d",&numChannels); fclose(fsize);
 float eta =  100*(float)numChannels/n;

  printf("================================================================================\n");
  printf("For %d sources - r = %1.3f bias = %1.3f sigma = %1.3f MAD = %1.3f | Norm: bias = %1.3f sigma = %1.3f NMAD = %1.3f eta = %1.2f percent\n", n, r, ave,sigma, MAD, ave_norm,sigma_norm,NMAD,eta);
 printf("================================================================================\n");
 //// LATEX FRIENDLY
 printf("%s  %1.3f &  %1.3f &  %1.3f &  %1.3f & %1.3f &  %1.2f \n", infile , r, sigma, MAD,sigma_norm,NMAD,eta);

	/////////////////////////////////////////////////////////////////////////////////////// 

   float z_i, z_f, err_x[999], err_y[999], X[999], Y[999];
   int Z, Zcount,flag_y, span_x =0;
   char outfile[200],SDerr[10]; //show SD [s] or error [e] 
  
   colour = 4; //12; 

  int nbin = 10; //BIN = (float)j/nbin; bin = (int)BIN;
  //printf("nbin = %d, BIN = %1.1f, bin = %d\n", nbin, BIN, bin);
 
  system("cp data1.co all.co");
  system("sort -n -k 1 all.co > all_z.as"); // without limits
  sprintf(systemCall,"wc -l all_z.as > /tmp/filesize"); system(systemCall);
  fsize = fopen("/tmp/filesize","r"); fscanf(fsize,"%d",&numChannels); fclose(fsize);
  float bin_start = 0, bin_end = 4.0;
  int inc = 1;

  //numChannels = j;
  Zcount = numChannels/bin;
  printf("numChannels = %d => %d bins of %d with remainder %d\n",  numChannels, Zcount, bin, numChannels - (Zcount*bin));

   strcpy(SDerr,"s"); // show error
    bin_equal(z_i,z_f,err_x,err_y,X,Y,up,dn,left,right,Z,Zcount,numChannels,bin,flag_y,outfile,f3,f1,f2,f4,k,i,x,y,n,N,N2,VAL,val,V,mean,prob,E,e,expec,v,var,med,ave,sigma,VAL2,val2,V2,mean2,prob2,E2,e2,expec2,v2,var2,ave2,sigma2,systemCall,fsize,sigma_string,span_x,SDerr,censor); cpgslw(3); cpgsci(colour); cpgerrx(Zcount,left,right,Y,1.);cpgerry(Zcount,X,up,dn,1.); cpgslw(3); 

   cpgsch(text);cpgsci(14);
  // draw x = y
   cpgslw(5); cpgsls(2);cpgmove(xbottom,xbottom); cpgdraw(xtop,xtop);
   
  cpgsch(text); cpgsci(1);cpgslw(3);  cpgsls(1);

  cpgbox("bsnct",0.,0.,"bcsnt",0.,0.); // on top of the greyscale
  cpgmtxt("L",2.5,0.5,0.5,"Photometric redshift, z\\dphot\\u");
  cpgmtxt("B",2.2,0.5,0.5,"Spectroscopic redshift, z\\dspec");
 
  ///////////// INSET SHOWING HISTO ///////////////////
  cpgsls(1);cpgslw(3); cpgsci(1);
  //xbottom = -2.99; xtop = 2.99; ybottom = 0; ytop = 29;
  xbottom = -0.99; xtop = 0.99; ybottom = 0; ytop = 449;
  nbin = 60;
  xstart =  xbottom + (xtop-xbottom)/24, ystart =  ytop-(ytop-ybottom)/6;
  
  cpgsvp(x1+0.11,x1+0.45,y1+0.5,y2-0.01);
  cpgswin(xbottom, xtop, ybottom, ytop); 
  cpgsch(0.8*text); cpgsci(1);
  //cpgtext(xstart,ystart,"LARGESS");
  
  cpgsci(15);
  cpghist(j,count,xbottom,xtop,nbin,3); // but not working?? BECAUSE cpgsci(0); ABOVE!!!
  cpgsci(colour);cpghist(j,count,xbottom,xtop,nbin,1); 
  
  cpgsls(1);cpgslw(3); cpgsci(1);
  cpgbox("bstcn",0.,0.,"bsntc",200.,5.); 
   
  ////////////////////////////////////
  xstart =  xtop + (xtop-xbottom)/16; 
    //xstart =  xbottom + (xtop-xbottom)/16; 
    ystart =  ytop-(ytop-ybottom)/8; yskip = (ytop-ybottom)/8;
  cpgsci(colour);
  //  sprintf(label,"\\gs\\dNMAD\\u = %1.3f", NMAD); cpgtext(xstart+0.01*xskip, ystart-0*yskip,label);
   sprintf(label,"n = %d per bin", bin); cpgtext(xstart+0.01*xskip, ystart,label);
   sprintf(label,"r = %1.3f", r); cpgtext(xstart+0.01*xskip, ystart-1.5*yskip,label);

  sprintf(systemCall,"gv %s &", ps_out);if (strcmp(format, "x") != 0) system(systemCall);
 printf("Written to %s\n", ps_out);


cpgend();

}
