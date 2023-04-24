#include <stdio.h>

// écrire une fonction qui remplit de 0 un tableau d'présent dans une classe M
#define BOARD_SIZE 11
#define VIE 5



typedef	struct Joueur
{
	char board[BOARD_SIZE][BOARD_SIZE]; // grille qui va contenir les bateaux 
	char vierge[BOARD_SIZE][BOARD_SIZE]; //grille permettant de voir ou le joueur va tirer
	int vie;
	char* nom; 
}joueur;

int verif(joueur* J, int x, int y, int r, int taille);
void initPlayer(joueur J1, joueur J2);
void initBoard(joueur* J);
void placeShips(joueur* J);
int playGame(joueur J1, joueur J2);
void afficher (joueur J);
int tirer (joueur J, joueur A);