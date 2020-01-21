#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"



Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}



Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}




void ajouter_animal(int x, int y, Animal **liste_animal) {

 	Animal *np;
	assert(liste_animal);//test de la presence de liste

	if((x<0) || (x>=SIZE_X) || (y<0) || (y>=SIZE_Y)){
		return; //si x ou y ne respecte pas les bornes 0->SIZE_X/Y
	}

	np=creer_animal(x,y,energie);
	assert(np);//test de reussite de la creation de l'animal
	*liste_animal=ajouter_en_tete_animal(*liste_animal,np);	//ajout en tete de liste
}


void enlever_animal(Animal **liste, Animal *animal) {
  assert(liste);
  assert(animal);

  Animal *preced=*liste;
  if (animal !=*liste ){
    /* Trouver le précédent*/
    while(preced->suivant != animal )
      preced=preced->suivant;
    /* refaire le chainage */
    preced->suivant=animal->suivant;
  }else /* ou sinon, changer la tete*/
    *liste=animal->suivant;
  

  free(animal);
  return ;
}


Animal* liberer_liste_animaux(Animal *liste) {
	 Animal *np=liste;
		if(!liste){
			return 0;
		}
		
		do{
			np=liste->suivant;
			free(liste);
			liste=np;
		}while(liste);

  	return liste;
}

unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}


unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la = la->suivant;
  }
  return cpt;
}


void bouger_animaux(Animal *la) {

	Animal *a;
	
	for (a=la; a!=NULL; a=a->suivant){
		a->x=(a->x + a->dir[0]) %SIZE_X;//on bouge le x, le modulo SIZE_X permet de respecter le monde
		a->y=(a->y + a->dir[1]) %SIZE_Y;//on bouge le y, le modulo SIZE_Y permet de respecter le monde

		if(rand()*1. /RAND_MAX<=p_ch_dir){ //si la proba de changer de direc est respectee
			a->dir[0]= (-1)+rand()%3;//on change la future direction x
			a->dir[1]= (-1)+rand()%3;//on change la future direction y
		}
	}
}




void reproduce(Animal **liste_animal) {

	assert(liste_animal); //verification de la presence de la liste_animal

	Animal *ani=*liste_animal;
	
	while(ani){
		if(rand()/(float)RAND_MAX < p_reproduce){//si la proba de reproduction est respectée
			ajouter_animal(ani->x, ani->y, liste_animal); //naissance
		}
		ani=ani->suivant; //passage au suivant
	}

}


void rafraichir_proies(Animal **liste_proie) {
  /* deplacement et mise a jour de l'energie */
	assert(liste_proie);//test presence liste

	bouger_animaux(*liste_proie);//on bouge les proies
	
	Animal *a=*liste_proie;
	Animal *cp;
	for(a=*liste_proie; a!=NULL;){
		a->energie = a->energie-d_proie;//on baisse l'energie des proies
		if(a->energie<=0){ //si l'energie est inf a 0
			cp=a->suivant;
			enlever_animal(liste_proie,a); //l'animal meurt
			a=cp;
		}else{
			a=a->suivant; //sinon on passe au suivant
		}
	
	}
  /* gestion de la reproduction */
	reproduce(liste_proie); //on reproduit les proies
}


Animal *animal_en_XY(Animal *l, int x, int y) {
 	
	Animal *a=l;
	
	while(a){
		if(a->x == x && a->y==y){
			return a; //renvoi de l'animal si il est sur la case
		}
		a=a->suivant;// on cherche l'animal suivant sinon
	}
  return NULL;
} 


void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {

  /* deplacement et mise a jour de l'energie */
	assert(liste_proie);//test de la presence de liste
	

	bouger_animaux(*liste_predateur); //on bouge les predateurs
	
	Animal *a;
	Animal *proie;
	Animal *cp;
	for(a=*liste_predateur; a!=NULL;){
		if((proie=animal_en_XY(*liste_proie, a->x, a->y))!=NULL){ //si il y a une proie
			if(rand()*1.0/RAND_MAX<=p_manger){ //si la probabilité est inf a p_manger

				enlever_animal(liste_proie,proie); //la proie est mangée
			}
		}
		a->energie = a->energie-d_predateur; //baisse de l'energie du predateur
		if(a->energie<=0){ //si l'energie est inf a 0
			cp=a->suivant;
			enlever_animal(liste_predateur,a); //mort du predateur
			a=cp;
		}else{
			a=a->suivant; //sinon on continue
		}

	}
  /* gestion de la reproduction */
	reproduce(liste_predateur); //on reproduit les predateurs
}



void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  static int cpt = 0;  /* Compteurs d'appels a la fonction */
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  int nbpred=0,nbproie=0;
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ++nbproie;
    if (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) {
      ecosys[pa->x][pa->y] = '*';
    } else {
      printf("Avertissement : Une proie avec energie %f est hors l'ecosysteme a (%d, %d).\n", 
             pa->energie, pa->x, pa->y);
    }
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
    ++nbpred;
    if (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    } else {
      printf("Avertissement : Un predateur avec energie %f est hors l'ecosysteme a (%d, %d).\n",
             pa->energie, pa->x, pa->y);
    }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  printf("Cpt : %5d\tNb proies : %5d\tNb predateurs : %5d\n", cpt, nbproie, nbpred);
  ++cpt;
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}
