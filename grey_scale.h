void grey_scale(FILE *f3, float x[], float y[],int p,int q, int i,int j, int colour, float xbottom, float xtop, float ybottom, float ytop, float yskip, float text, int no_y, int no_x, float x_size, float y_size, float px1, float px2, float py1, float py2, float NO_Y, char shade[], char grey[], float NO_XY, int no_xy, int pn[], int nb[][no_y], int max_nb, int log_shade, int start_shade, int stop_shade, int span_shade, int col[], float NB[][no_y], float MAX_NB, float inc1,float inc2,float key_x,float key_y,float key_skip, char key_corn[], char label_b[][20], float kx, float ky, int *M, int maxNB){  // The second and subsequent dimensions must be given:

   if (strcmp(key_corn, "tl") == 0) {key_x = xbottom + (xtop-xbottom)/kx; key_y =  ytop-(ytop-ybottom)/ky;}
  if (strcmp(key_corn, "tr") == 0) {key_x = xtop - (xtop-xbottom)/kx; key_y =  ytop-(ytop-ybottom)/ky;}
  if (strcmp(key_corn, "bl") == 0) {key_x =  xbottom + (xtop-xbottom)/kx; key_y = ybottom+(ytop-ybottom)/ky;}
  if (strcmp(key_corn, "br") == 0){key_x = xtop - (xtop-xbottom)/kx; key_y = ybottom + (ytop-ybottom)/ky;} 
  //else {key_x = xtop - (xtop-xbottom)/4.8; key_y = ybottom;} //+(ytop-ybottom)/60;} // may need adjusted
  //key_skip = yskip/1.5; //(ytop-ybottom)/12;

  //the simplest way to assign the initial values as zero is using for loop
  for (p = 1; p<= no_x; p++){for (q = 1; q<=no_y; q++) nb[p][q] = 0;} // initilaisation
 
  for (p =1; p <= no_x; p++){ // vert strips first
     f3 = fopen("all.co","r");for (i = 0; fscanf(f3,"%f %f", &x[i], &y[i]) !=EOF; i++) { 
       px1 = xbottom+(((float)p-1)*x_size); px2= xbottom+(float)p*x_size; //starting from 1 cf 0
       if (x[i] >= px1 && x[i] <= px2){ 
	 for (q =1; q <= no_y; q++){ //horiz 
	   py1 = ybottom+(((float)q-1)*y_size); py2= ybottom+(float)q*y_size;
	   if (y[i] >= py1 && y[i] <= py2) nb[p][q] = nb[p][q]+1;
	   
	   if (p ==1 && q==1) max_nb = nb[p][q];
	   else{if(max_nb<nb[p][q]) max_nb=nb[p][q];} //need a pointer to get this out?
	 
	   // if( nb[p][q] ==1) 
	   // printf("p = %d  q = %d  no_x = %d [%1.2f] no_y = %d  [%1.2f]  x = %1.2f - %1.2f  y = %1.2f - %1.2f | nb[%d][%d] = %d max = %d\n", p, q, no_x,x_size, no_y,y_size, px1, px2, py1,py2, p,q, nb[p][q], max_nb);
	     M = &max_nb; maxNB = *M; // need pointer to get max out
	 }
      }
    }
  }

  //  printf("nb[2][8] = %d nb[3][18] = %d\n", nb[2][8], nb[3][18]); //should give 4 and 0

  for (p =1; p <= no_x; p++){ 
    px1 = xbottom+(((float)p-1)*x_size); px2= xbottom+(float)p*x_size;
    for (q =1; q <= no_y; q++){
      py1 = ybottom+(((float)q-1)*y_size); py2= ybottom+(float)q*y_size;
   
      //cpgsfs(2); //outline // works
      //cpgsfs(1); // solid
      // log_shade =0;
      if (log_shade == 1) {
	if (nb[p][q]>1) NB[p][q] = log10((float)nb[p][q]);
	else if (nb[p][q]==1) NB[p][q] = (float)nb[p][q]; // need this or bins with single get missed
	else NB[p][q] = 0;
      	MAX_NB = log10((float)maxNB);
      }
        else {NB[p][q] = (float)nb[p][q]; MAX_NB = maxNB;}
       ///////////// shades of grey ////////////
       cpgscr(16,0.0,0.0,0.0);cpgscr(17,0.1,0.1,0.1); cpgscr(18,0.2,0.2,0.2);cpgscr(19,0.3,0.3,0.3);cpgscr(20,0.4,0.4,0.4); cpgscr(21,0.5,0.5,0.5); cpgscr(22,0.6,0.6,0.6);cpgscr(23,0.7,0.7,0.7);cpgscr(24,0.7,0.7,0.7);cpgscr(25,0.8,0.8,0.8);cpgscr(26,0.9,0.9,0.9); cpgscr(27,1.0,1.0,1.0);// black to white 
       // want to generalise, so can have 5, 10 whatever steps ////
       span_shade = start_shade - stop_shade;
       
       for (j = start_shade; j >=stop_shade; j--){ 
	 inc1 = (((float)j-1)/span_shade); inc2 = ((float)j/span_shade);
	 
	 if (NB[p][q] > inc1*MAX_NB && NB[p][q] <= inc2*MAX_NB) colour = 27 -j; // ps in reverse  15+j;
	 //else  colour = 0; 
	 if (NB[p][q] ==0) colour = 0; //just to get rid of little stray lines

	 if (log_shade == 1) sprintf(label_b[j], "%1.0f - %1.0f", pow(10,inc1*MAX_NB), pow(10,inc2*MAX_NB)); 
	 else sprintf(label_b[j], "%1.0f - %1.0f", inc1*MAX_NB, inc2*MAX_NB); 
	
	 // if(p==2 && q ==8)
	 // if(p==3 && q ==16)
	 //   printf("p = %d q = %d | j = %d max = %1.0f cf. %d  from %1.2f to %1.2f nb = %1.0f cf. %d  colour = %d\n", p,q, j, MAX_NB, maxNB, inc1*MAX_NB,inc2*MAX_NB, NB[p][q], nb[p][q], colour);
	 cpgsci(colour); cpgrect(px1,px2,py1,py2);	
	     }
    }
  }
  // need to print key after as gets lost
  cpgslw(3);  
  
  // if (log_shade == 1) {stop_shade = 2; key_y = key_y-key_skip;} // to avoid 1-1
  for (j = start_shade; j >=stop_shade; j--){ // don't know shy not matching the above
    colour = 27 - j;
    cpgsci(colour); cpgsch(0.7*text);cpgtext(key_x, key_y+((j-1)*key_skip),label_b[j]);
  }
  
}
