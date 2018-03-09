#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include<fcntl.h>
#include<time.h>
#include "solution.h"

//#define AFFICHE 

int **matrice_temps;
int *tableau_livraison_min;
int *tableau_livraison_max;
int nb_sommets;

/********************************************************************************************************************************/
void ecrire_solution(char* fileName, int* solution, int valeur, double time){
/********************************************************************************************************************************/
FILE* outputFile;
  if( (outputFile=fopen("toutes_les_solutions", "a")) == NULL ){
	printf("Fichier n'existe pas: %s\n", fileName);
	exit(2);
  }
  fprintf(outputFile,"%s\t",fileName);
  fprintf(outputFile,"%d\t",valeur);
  fprintf(outputFile,"%lf\t",time);
  int i;
  for(i=0;i<nb_sommets-1;i++){
	fprintf(outputFile,"%d\t",solution[i]);
  }
  fprintf(outputFile,"\n");
}



/********************************************************************************************************************************/
void lecture_fichier(char* fileName){
/********************************************************************************************************************************/


  /* fichier d'entree */
  FILE* inputFile;
  int i, j, v1, v2;
  char c[255];
  char str[255];

  if( (inputFile=fopen(fileName, "r")) == NULL ){
    printf("Fichier n'existe pas: %s\n", fileName);
    exit(2);
  }

  fscanf(inputFile,"%d", &nb_sommets);
  printf("%d\n",nb_sommets);


  tableau_livraison_min=malloc(nb_sommets*sizeof(int*));
  tableau_livraison_max=malloc(nb_sommets*sizeof(int*));
  matrice_temps=malloc(nb_sommets*sizeof(int*));

  for(i=0;i<nb_sommets;i++){
	  matrice_temps[i]=malloc(nb_sommets*sizeof(int));
	  for(j=0;j<nb_sommets;j++){
		  matrice_temps[i][j]=0;
	  }
  }

  fscanf(inputFile,"%s",c);
  printf("%s\n",c);

  for(i=0; i<nb_sommets; i++){
	int v1,v2,v3;
	if(fscanf(inputFile, "%d %d %d", &v1, &v2,&v3) !=3){
	  puts("readInputGraph: Error");
	  exit(4);
	}
	if(v1!=i) {printf("probleme dans les donnees\n");    exit(5);}
	if(v2>v3) {printf("max plus petit que min\n");    exit(5);}
	tableau_livraison_min[i]=v2;
	tableau_livraison_max[i]=v3;
	printf("%d %d %d\n",v1,v2,v3);
  }

  fscanf(inputFile,"%s",c);
  printf("%s\n",c);

  for(i=0; i<nb_sommets*nb_sommets-nb_sommets; i++){
	int v1,v2,v3;
	if(fscanf(inputFile, "%d %d %d", &v1, &v2,&v3) !=3){
	  puts("readInputGraph: Error");
	  exit(4);
	}

	matrice_temps[v1][v2]=v3;
	printf("%d %d %d\n",v1,v2,v3);
  }

  fclose(inputFile);
}

/********************************************************************************************************************************/
int evaluation(int* solution){
/********************************************************************************************************************************/
	int i,j;
	int valeur=0;
	int pred=0;
	int eval=0;
	for(i=0;i<nb_sommets-1;i++){
		int courant=solution[i];
		valeur+=matrice_temps[pred][courant];
		if(valeur<tableau_livraison_min[courant]){eval+=tableau_livraison_min[courant]-valeur;}
		if(valeur>tableau_livraison_max[courant]){eval+=valeur-tableau_livraison_max[courant];}
	
#ifdef AFFICHE
		printf("passage à %d à l'heure %d retard cumulé %d\n",courant,valeur,eval);
#endif

		pred=courant;
	}
	return eval;
}

/********************************************************************************************************************************/
void affiche_tableaux(){
/********************************************************************************************************************************/
    int i,j;
    printf("affiche \n");
    for(i=0;i<nb_sommets;i++){
        for(j=0;j<nb_sommets;j++){
            printf("%d\t",matrice_temps[i][j]);
        }
        
        printf("\n");
    }
} 



/********************************************************************************************************************************/
void liberer(){
/********************************************************************************************************************************/
	int i;

    free(tableau_livraison_min);
    free(tableau_livraison_max);

    for(i=0;i<nb_sommets;i++){
        free(matrice_temps[i]);
    }
    
    free(matrice_temps);
}

/********************************************************************************************************************************/
int main(int argc, char *argv[]){
/********************************************************************************************************************************/
  if(argc!=3){printf("nom du fichier ? nombre de boucle d'optimistaion?\n\n");return 1;}
  lecture_fichier(argv[1]);
  int nb_boucles = atoi(argv[2]);
  int i;
  //creation d une solution aleatoire
#ifdef AFFICHE
  affiche_tableaux();
#endif
  
  int* solution=malloc((nb_sommets)*sizeof(int));
  
  for(i=0;i<nb_sommets;i++){
    solution[i]=i;
  }
  long int val_solu = evaluation(solution+1);
  clock_t t1, t2;
  //printf("\nsolution fournit: %ld\n",val_solu);
  
  /*1ere methode*/
  /*t1 = clock();
  
  int *tableau_glt = glouton_fonct(tableau_livraison_min, tableau_livraison_max, matrice_temps, nb_sommets);
  long int retard_glt = evaluation(tableau_glt+1);
  
  t2 = clock();
  
  ecrire_solution(argv[1], tableau_glt+1, retard_glt, (float)(t2-t1)/CLOCKS_PER_SEC);
  printf("Sequence methode 1° : \n"); 
  affiche(tableau_glt,nb_sommets);
  printf("-Retard séquence 1° : %ld\n\n",retard_glt);*/

  
  /*Algo Recherche Local*/
  t1 = clock();
  
  int *tableau_glt_mod = recherche_local(solution, tableau_livraison_min, tableau_livraison_max, matrice_temps, nb_sommets);
  long int retard_glt_mod = evaluation(tableau_glt_mod+1);
  //t2 = clock();
  
  //ecrire_solution(argv[1], tableau_glt_mod+1, retard_glt_mod, (float)(t2-t1)/CLOCKS_PER_SEC);
  //printf("Sequence methode 2° :\n"); 
  //affiche(tableau_glt_mod,nb_sommets);
  //printf("-Retard séquence 2° : %ld\n\n",retard_glt_mod);
  
  
  /*3eme methode (2eme methode modifiée)*/
  //t1 = clock();
  
  int *tableau_glt_final;
  long int retard_glt_final = retard_glt_mod;
  int meilleur_solu[nb_sommets];
  transfer(meilleur_solu, tableau_glt_mod, nb_sommets);
  
  for(i=0;i<nb_boucles;i++){
      if(retard_glt_final == 0)break;
      tableau_glt_final = recherche_local(tableau_glt_mod, tableau_livraison_min, tableau_livraison_max, matrice_temps, nb_sommets);
      free(tableau_glt_mod);
      tableau_glt_mod = tableau_glt_final;
      
      if(evaluation(tableau_glt_final+1) < retard_glt_final){
	    transfer(meilleur_solu, tableau_glt_mod, nb_sommets);
	    retard_glt_final = evaluation(tableau_glt_final+1);
      }
  }
  
  t2 = clock();
  
  printf("\nSequence Recherche Local :\n"); 
  affiche(meilleur_solu,nb_sommets);
  printf("-Retard recherche local : %ld\n\n",retard_glt_final);
  ecrire_solution(argv[1], meilleur_solu+1, retard_glt_final, (float)(t2-t1)/CLOCKS_PER_SEC);
  
  
  free(solution);
  liberer();
  
}
