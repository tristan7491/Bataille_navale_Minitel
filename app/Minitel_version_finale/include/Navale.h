#include <stdio.h>

// écrire une fonction qui remplit de 0 un tableau d'présent dans une classe M
#define BOARD_SIZE 10
#define VIE 5



typedef	struct Joueur
{
	char board[BOARD_SIZE][BOARD_SIZE]; // grille qui va contenir les bateaux 
	char vierge[BOARD_SIZE][BOARD_SIZE]; //grille permettant de voir ou le joueur va tirer
	int vie; // vie joueur
	int vie_a; // vie adversaire
}joueur;

int verif(joueur* J, int x, int y, int r, int taille);
void initPlayer(joueur J1, joueur J2);
void initBoard(joueur* J);
void placeShips(joueur* J);
int playGame(joueur* J1);

void read_lora(char* msg);

#undef STDIO_UART_DEV
#define STDIO_UART_DEV      	UART_DEV(1)

#define UART  UART_DEV(1)
#define BAUDRATE (1200U)

typedef struct {
    int x;
    int y;
} Cursor;

void filltab(char tab[10][10], char c);
Cursor move(Cursor curr, int dir);
Cursor input(void);
void navaleMain(void);

void grille(char jo[10][10], int mode);

void enableScreen(int val);
int input_orientation(void);