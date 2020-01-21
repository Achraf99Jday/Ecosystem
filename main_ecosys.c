#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 50000


  /* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.01;
float d_proie=1;
float d_predateur=1;
float p_manger=0.2;
float p_reproduce=0.03;
float energie=50;


int main(void) {

	Animal *liste_proie=NULL;
	Animal *liste_predateur=NULL;
	int i;

	for(i=0; i<NB_PROIES; i++){
		ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y, &liste_proie); //ajout des proies
	}
	for(i=0; i<NB_PREDATEURS; i++){
		ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y, &liste_predateur); //ajout des predateurs
	}
	
	do{
		if (liste_proie==NULL && liste_predateur==NULL){
			break;
		}

		printf("Nombre de proies : %d \n",compte_animal_rec(liste_proie)); //compte proie
		printf("Nombre de predateurs : %d\n",compte_animal_rec(liste_predateur));//compte pred

		afficher_ecosys(liste_proie, liste_predateur);//affichage ecosysteme
		rafraichir_proies(&liste_proie);
		rafraichir_predateurs(&liste_predateur,&liste_proie);
		usleep(T_WAIT);
		clear_screen();
		
		
	}while(compte_animal_rec(liste_proie)!=0 && compte_animal_rec(liste_predateur)!=0);

	liberer_liste_animaux(liste_predateur); 
	liberer_liste_animaux(liste_proie);
	
	printf("\nEcosysteme mort\n\n");

  	return 0;
}
