void prob_print (long double prob, int q, int i, char cut_string[], char cut1[], char cut2[], char cut3[], int dp){

  if (dp==1) sprintf(cut_string,"%1.1Le", prob);
  if (dp==2) sprintf(cut_string,"%1.2Le", prob);
  if (dp==3) sprintf(cut_string,"%1.3Le", prob);
  if (dp==4) sprintf(cut_string,"%1.4Le", prob);

  //  printf("%s\n",cut_string);
  q = 1;
  while (cut_string[q]){
    if (prob > 1e-9) {char *trun = &cut_string[q+1];
      if (cut_string[q] == '0') strcpy(cut2, trun);
    } // print from the 2nd character after 'e', i.e. skipping the +
      else {char *trun = &cut_string[q+1];
      if (cut_string[q] == '-') strcpy(cut2, trun);
      //printf("%d  %c\n", q, cut_string[q]);
    }
    q++; // need this or hangs
  } //printf("%s\n",cut2);
  if (prob < 1) sprintf(cut3,"%s", cut2); // printf("%1.3Le cut_string = %s cut2 = %s, cut3 = %s\n",prob, cut_string, cut2,cut3);

  i = 1;
  while (cut_string[i]){
    char *trun = &cut_string[i-2-dp]; // print the stuff before 'e' -4 FOR DP 2
    if (cut_string[i] == 'e') // preamble before exponent so leave this 
    //   char *trun = &cut_string[i+2]; // only works backwards
    //  if (cut_string[i] == '.')
      strncpy(cut1, trun,i);  //printf("cut_string = %s cut1 = %s cut2 = %s, cut3 = %s\n",cut_string,cut1, cut2,cut3);
    i++; // WANT TO PRINT THE STUFF BEFORE 'e'
  }
}
