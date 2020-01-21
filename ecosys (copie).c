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
 	/* A COMPLETER*/
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
 
 /* A COMPLETER*/
  return NULL;
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
 /* A COMPLETER */
}


void reproduce(Animal **liste_animal) {
 /* A COMPLETER */

}


void rafraichir_proies(Animal **liste_proie) {
 /* A COMPLETER */
  /* deplacement et mise a jour de l'energie */
  /* gestion de la reproduction */
}


Animal *animal_en_XY(Animal *l, int x, int y) {
 /* A COMPLETER */
  return NULL;
} 


void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
   /* A COMPLETER */
  /* deplacement et mise a jour de l'energie */
  /* gestion de la reproduction */
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
