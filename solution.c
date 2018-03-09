#include<stdio.h>
#include<stdlib.h> 
#include<string.h>

int *insert(int *tmp, int ins_nub, int nb){
  int *res = malloc(sizeof(int)*nb);
  res[0] = tmp[0];
  res[1] = ins_nub;
  int i = 2;
  
  for (i=2; i < nb; i++){
      res[i] = tmp[i-1];
  }
  
  return res;
}

int evaluer(int *tab, int *tmin, int *tmax, int **mt, int nb){
  int time = 0;
  int retard = 0;
  int i=1;
  
  for(i=1;i<nb;i++){
      int sor = tab[i-1];
      int des = tab[i];
      time+=mt[sor][des]; //time = time + mt[sor][des];
      if(time < tmin[des])retard += tmin[des] - time;
      else if(time > tmax[des])retard += time -tmax[des];
  }
  
  return retard;
}

int *transfer(int *destination, int *sortie,int nb){
  int i=0;
  
  for (i=0; i < nb; i++){
      destination[i] = sortie[i];
  }
  
  return destination;
}

void affiche(int *tab, int nb){
  int i=0;
  
  for(i=0;i<nb;i++){
    printf("%d ",tab[i]);
  }
  printf("\n");
}


int *recherche_local(int *tab, int *tmin, int *tmax, int **mt, int nb){
  
    if(nb == 1){
        int *res = malloc(sizeof(int));
        res[0]=0;
        return res;
    }
    
    int *tmp = recherche_local(tab,tmin,tmax,mt,nb-1);
    int min_retard = 100000000;
    int *res = malloc(sizeof(int)*nb);
    int i=0;
    
    int *tab_eval = insert(tmp,tab[nb-1],nb);
    
    for(i=0;i<nb;i++){
        int min_re_tmp = evaluer(tab_eval,tmin,tmax,mt,nb);
        if(min_re_tmp < min_retard){
            min_retard = min_re_tmp;
            transfer(res,tab_eval,nb);
        }
        if(i < nb-2){
            int exchange = tab_eval[i+1];
            tab_eval[i+1] = tab_eval[i+2];
            tab_eval[i+2] = exchange;
        }
    }
    
    /*printf("Sequence methode 2° :"); 
    affiche(res,nb);*/
    free(tab_eval);
    return res;
}


int *glouton_fonct(int *tmin, int *tmax, int **mt, int nb){
      int ems[nb-1];
      int i = 0;
      
      for (i=1; i<nb; i++){
	  ems[i-1] = i;
	
      }
      
      int *res = malloc(nb*sizeof(int));
      res[0] = 0;
      printf ("\nok\n");
      
      for (i=1;i < nb; i++){
	  int min = 100000000;
	  int j,numb_next;
	  for (j = 0; j < nb-1; j++){
	      if(ems[j]==0)continue;
	      res[i] = ems[j];
	      int evl = evaluer(res,tmin,tmax,mt,i+1);
	      if(evl<min){
                numb_next = ems[j];
                min = evl;
	      }
	    
	  }
	  
	  res[i] = numb_next;
	  ems[numb_next-1] = 0;
	
      }
      /*printf("Sequence methode 1°:"); 
      affiche(res,nb);*/
      return res;
  
}


