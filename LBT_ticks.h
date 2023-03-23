#define   MAXSTRING        200  // don't want anything returned so void?

void LBT_ticks(float step, float tick[], float text_size, float main_tick_size, float sub_tick_size, float ytop_extra, float tick_label_off, float mult_tick_label_off, float sub_space, int p, int q, int tick_bottom,  int tick_top,  int log_tick, char step_string[], float xbottom, float ybottom, float xtop, float ytop, float xlimit){ // xlimit new to cut-off LBT

  for (q = 1; q < 1*sub_space; q++){ // subticks up to z = 1
    tick[q] = -4.446897829502e-03  +   8.289650394852e-02 * (float)q/sub_space  +  -1.237103890046e-03 * pow((float)q/sub_space,2)  +   9.370671119955e-04 * pow((float)q/sub_space,3);
    if (log_tick == 1) tick[q] = log10(tick[q]); else if (log_tick == 0) tick[q] = tick[q];
    cpgtick(xbottom, ytop, xtop, ytop, (tick[q]-xbottom)/(xtop-xbottom), sub_tick_size, sub_tick_size,  0, 0, "");
  }
  if (log_tick == 1) sub_tick_size = 0; // not to have any more subticks in z > 1 bins
	 
  for (p = 1; p < 7; p++){  // p is LBT and tick[p] is the redshift to place it at
    tick[p] =   -4.446897829502e-03  +   8.289650394852e-02 * p +  -1.237103890046e-03 * pow(p,2) +   9.370671119955e-04 * pow(p,3);
    if (log_tick == 1) tick[p] = log10(tick[p]); else  tick[p] = tick[p];
    cpgtick(xbottom, ytop, xtop, ytop, (tick[p]-xbottom)/(xtop-xbottom), main_tick_size, main_tick_size, 0, 0, "");
    sprintf(step_string," %1.0f",(float)p); //if (p > 9) tick_label_off = 1.3*tick_label_off;
    cpgsch(text_size);cpgtext(tick[p]+tick_label_off, ytop+ytop_extra, step_string);
  }  
  for (q = 1*sub_space; q < 7*sub_space; q++){ // subticks
    tick[q] = -4.446897829502e-03  +   8.289650394852e-02 * (float)q/sub_space +  -1.237103890046e-03 * pow((float)q/sub_space,2) +   9.370671119955e-04 * pow((float)q/sub_space,3);
    if (log_tick == 1) tick[q] = log10(tick[q]); else  tick[q] = tick[q];
    cpgtick(xbottom, ytop, xtop, ytop, (tick[q]-xbottom)/(xtop-xbottom), sub_tick_size, sub_tick_size,  0, 0, "");
  }
  for (p = 7; p < 12; p++){  
    tick[p] =  -4.149616260532e-04 +   3.292091465180e-01 * p  +  -1.727429496513e-01 * pow(p,2) +   4.258684243098e-02 * pow(p,3)  +  -4.289014131948e-03 * pow(p,4) +   1.608333608638e-04 * pow(p,5); 
    if (log_tick == 1) tick[p] = log10(tick[p]); else  tick[p] = tick[p];
    cpgtick(xbottom, ytop, xtop, ytop, (tick[p]-xbottom)/(xtop-xbottom), main_tick_size, main_tick_size, 0, 0, "");
    sprintf(step_string," %1.0f",(float)p);  if (p > 9) tick_label_off = mult_tick_label_off*tick_label_off; // as different offset for labels 10, 11, etc.
    cpgsch(text_size);cpgtext(tick[p]+tick_label_off, ytop+ytop_extra, step_string); 
  }
  for (q = 7*sub_space; q < 12*sub_space; q++){
    tick[q] =  -4.149616260532e-04 +   3.292091465180e-01 * (float)q/sub_space  +  -1.727429496513e-01 * pow((float)q/sub_space,2) +   4.258684243098e-02 * pow((float)q/sub_space,3)  +  -4.289014131948e-03 * pow((float)q/sub_space,4) +   1.608333608638e-04 * pow((float)q/sub_space,5); // this polynomial good up to z = 3.6, or 11.90 Gyr
    if (log_tick == 1) tick[q] = log10(tick[q]); else  tick[q] = tick[q];
    cpgtick(xbottom, ytop, xtop, ytop, (tick[q]-xbottom)/(xtop-xbottom), sub_tick_size, sub_tick_size,  0, 0, "");
  }

  if (xlimit >= 12){ // to define cut-off so doen't run off axis

    for (p = 12; p < 13; p++){ 
      tick[p] =   1.216682242137e+05  +  -3.915122151612e+04 * p +   4.725205811259e+03 * pow(p,2) +  -2.535180628746e+02 * pow(p,3) +   5.102232127098e+00 * pow(p,4);
      if (log_tick == 1) tick[p] = log10(tick[p]); else  tick[p] = tick[p];
      cpgtick(xbottom, ytop, xtop, ytop, (tick[p]-xbottom)/(xtop-xbottom), main_tick_size, main_tick_size, 0, 0, "");
      sprintf(step_string," %1.0f",(float)p); // if (p > 9) tick_label_off = 1.3*tick_label_off;
      cpgsch(text_size);cpgtext(tick[p]+tick_label_off, ytop+ytop_extra, step_string); 
    }
    for (q = 12*sub_space; q < 13*sub_space; q++){
      tick[q] = 1.216682242137e+05  +  -3.915122151612e+04 * (float)q/sub_space +   4.725205811259e+03 * pow((float)q/sub_space,2) +  -2.535180628746e+02 * pow((float)q/sub_space,3) +   5.102232127098e+00 * pow((float)q/sub_space,4); 
      if (log_tick == 1) tick[q] = log10(tick[q]); else  tick[q] = tick[q];
      cpgtick(xbottom, ytop, xtop, ytop, (tick[q]-xbottom)/(xtop-xbottom), sub_tick_size, sub_tick_size,  0, 0, "");  
    }
    if (xlimit >= 13){ 
      for (p = 13; p < 13.5; p++){  
	tick[p] =    1.267477098400e+05 +  -5.666310658141e+04 * p +   8.536987928883e+03 * pow(p,2) +  -5.383767720634e+02 * pow(p,3) +   1.225247627928e+01 * pow(p,4);
	if (log_tick == 1) tick[p] = log10(tick[p]); else  tick[p] = tick[p];
	cpgtick(xbottom, ytop, xtop, ytop, (tick[p]-xbottom)/(xtop-xbottom), main_tick_size, main_tick_size, 0, 0, "");
	sprintf(step_string," %1.0f",(float)p);  if (p > 9) tick_label_off = 1.3*tick_label_off;
	cpgsch(text_size);cpgtext(tick[p]+tick_label_off, ytop+ytop_extra, step_string);  
      }
      for (q = 13*sub_space; q < 13.5*sub_space; q++){
	tick[q] =  1.267477098400e+05 +  -5.666310658141e+04 * (float)q/sub_space +   8.536987928883e+03 * pow((float)q/sub_space,2) +  -5.383767720634e+02 * pow((float)q/sub_space,3) +   1.225247627928e+01 * pow((float)q/sub_space,4);
	if (log_tick == 1) tick[q] = log10(tick[q]); else  tick[q] = tick[q];
	cpgtick(xbottom, ytop, xtop, ytop, (tick[q]-xbottom)/(xtop-xbottom), sub_tick_size, sub_tick_size,  0, 0, "");     
      }
    }
  }

}
