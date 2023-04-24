#include <stdio.h>
#include "include/Navale.h"

int bateaux[VIE]={2,3,3,4,5};

//permet d'initier une grille de jeu a son éta initale 
void initBoard(joueur* J){
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			J->board[i][j] = '0';
			J->vierge[i][j] = '0';
		}
	}
}

//cette fonction permet de placer les bateaux sur une grille
void placeShips(joueur* J) {
    int x,y,r;
    int v = 0; 
	printf("********************* Placement des bateaux *********************\n");
    for ( int i=0; i<5; i++) {
		while ( v == 0) {
			printf("Entrer la ligne du bateau %d de taille %d:", i+1, bateaux[i]); 
			scanf("%d", &x);
			printf("Entrer la colonne du bateau %d de taille %d:", i+1, bateaux[i]); 
			scanf("%d", &y);
			printf("Entrer l'orientation du bateau %d de taille %d:", i+1, bateaux[i]); 
			scanf("%d", &r);

			v = verif(J, x, y, r, bateaux[i]);
		}
		if ( r==0) { /* HOrizontal */
			for (int j=0; j<bateaux[i]; j++) {
				J->board[x+j][y] = 1;
			}
		} else { /*Vertical */
			for (int j=0; j<bateaux[i]; j++) {
				J->board[x][y+j] = 1;
			}
	    }
		v = 0;		
	}
}

//permet de vérifier les conditions de placement des bateaux
int verif(joueur* J, int x, int y, int r, int taille) {
	int res = 1;
	if ( (x <0 )|| (y<0) || ( r == 0 && x+taille>=BOARD_SIZE) || ( r == 1 && y+taille>=BOARD_SIZE )) res = 0;
	if (r == 0) {
		for (int i=x; i<x+taille; i++) {
			if (J->board[i][y] == 1) res = 0;
		}
	} else {
		for (int i=y; i<y+taille; i++) {
			if (J->board[x][i] == 1) res = 0;
		}
	}
	if (res == 0) printf("les coordonnées ne sont pas valides\n");
	return res;
}

//cette fonction permet d'afficher les grilles et de jouer les coups
int playGame(joueur J1, joueur J2){
	//Conditions de victoire 
	int tour = 1;
	int x,y, verif;
	joueur J, A;
	J = J2;
	A = J1;
	while (A.vie !=0 || J.vie !=0){
		if (tour == 1) {
			J2 = J;
			J1 = A;
			J = J1;
			A = J2;
		} else {
			J1 = J;
			J2 = A;
			J = J2;
			A = J1;
		}
		//Affichage des grilles
		printf("*********************Changement de tour*********************\n");
		afficher(J);
		//Selection de la cible 
		verif = 0;
		while (verif == 0) {
		printf("Entrer la ligne du bateau z attaquer "); 
		scanf("%d", &x);	
   		printf("Entrer la colonne du bateau a attaquer "); 
		scanf("%d", &y);
		if ( x == 0 || y ==0 || x >=BOARD_SIZE || y>=BOARD_SIZE) {
			printf("les coordonnées ne sont pas valides \n");
		}
		else if (A.board[x][y] ==1){
			printf("Touche \n");
			A.vie--;
			A.board[x][y] = 3;
			J.vierge[x][y] = 3;
			verif = 1;
   		} else {
			printf("Manque \n");
			A.board[x][y] = 2;
			J.vierge[x][y] = 2;
			verif = 1;
		}
		}
		tour *= -1;
	}
	printf("la partie est fini");
	return 1;
}

//affiche les grilles d'un joueur
void afficher (joueur J) {
	printf("*********************Votre grille********************\n");
	for (int i=0; i<BOARD_SIZE; i++) {
        for (int j=0; j<BOARD_SIZE; j++) {
	        printf(" | %c | ", J.board[i][j]);
	   	}
	    printf("\n");
    }

    printf("*********************grille de l'adversaire*********************\n");
    for (int i=0; i<BOARD_SIZE; i++) {
        for (int j=0; j<BOARD_SIZE; j++) {
	        printf(" | %c | ",J.vierge[i][j]);
		}
		printf("\n");
	}
}

/*
int main() {
    joueur J1,J2;
    J1.vie = 19;
    J2.vie= 19;
	initBoard(&J1); 
    initBoard(&J2);

	placeShips(&J1);
	placeShips(&J2);
	playGame(J1, J2);
	return 0;
}
*/
