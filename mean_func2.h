void mean_func2(int numChannels, int i, char systemCall[], FILE *fsize, float *mean2, float VAL2[], float *val2, float V2, FILE *f1, float prob2, float E2, float e2[], float expec2, float v2[], float var2, char sigma_string[], float med){

  ///////////////// determines size of array /////////////////////////////////////////////
  system("wc -l mean_out2.txt > /tmp/filesize");  // will create and remove  mean_out.txt each time
  fsize = fopen("/tmp/filesize","r");
  fscanf(fsize,"%d",&numChannels);
  fclose(fsize);
   /////////////////////////////////////////////////////////////////////////////////////////
      
   f1= fopen("mean_out2.txt","r"); 
   i=0; // THIS IS VERY IMPORTANT!!!! 
    while(fscanf(f1,"%f",&VAL2[i])!=EOF){ 
     prob2 = 1/(float)numChannels;
      e2[i] = (VAL2[i]*prob2);
      i++;
      // printf("Val = %1.2f  numChannels = %d prob = %1.2e e[%d] = %1.2e\n", VAL2[i], numChannels, prob2, i, e[i]);
   }fclose(f1); // avoid memory leaks
   expec2 = 0;  
   for(i=0; i<numChannels; i++){expec2 += e2[i];
  }
   sprintf(sigma_string,"%s","sigma");

f1= fopen("mean_out2.txt","r"); 
 i=0; // THIS IS VERY IMPORTANT!!!! 
    while(fscanf(f1,"%f",&VAL2[i])!=EOF){ 
     prob2 = 1/(float)numChannels;
      v2[i] =  (VAL2[i] - expec2)*(VAL2[i] - expec2)*prob2;
      i++;
   }fclose(f1); // avoid memory leaks
var2 = 0;  
for(i=0; i<numChannels; i++) var2 += v2[i];
printf("Mean is %1.3f +/- %1.3g [where sigma = %1.3g] for n = %d\n", expec2, sqrt(var2/numChannels), sqrt(var2), numChannels);
system("rm mean_out2.txt"); // for next time
system("rm mean_values2.txt");
 f1 = fopen ("mean_values2.txt", "a"); fprintf(f1,"%1.3f %1.3f %d\n", expec2, sqrt(var2),numChannels); fclose(f1);
 }
