#include <stdio.h>
#include "include/Navale.h"
#include "include/Minitel.h"


/* Include lecture UART */
#include <stdlib.h>
#include "msg.h"
#include "thread.h"
#include "fmt.h"
#include "periph/uart.h"
#include "stdio_uart.h"

#include "xtimer.h"

#include "lora_p2p.h"

Cursor cursor_pos;
int courant;
int bateaux[VIE]={2,3,3,4,5};
struct netdev* dev;
joueur J1;

char buffer[10];

int tour = 1;

int placement=0;
extern volatile uint8_t flag_rx_lora; //lora rx message avilable

extern char message[32]; //content of lora rx message


//permet d'initier une grille de jeu a son éta initale 
void initBoard(joueur* J){
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE; j++) {
			J->board[i][j] = '-';
			J->vierge[i][j] = '-';
		}
	}
}

//cette fonction permet de placer les bateaux sur une grille
void placeShips(joueur* J) {
    int x,y,r;
    int v = 0; 
    
    moveCursorTo(24,2);
	printf("              ");
	moveCursorTo(24,2);
	printf("VOTRE GRILLE");
    
    for ( int i=0; i<5; i++) {
    	moveCursorTo(24,4);
    printf("           ");
    moveCursorTo(24,4);
    printf("PLACER (%d CASES)\n",bateaux[i]);
		while ( v == 0) {
			moveCursorTo(24,5);
			printf("               ");
			moveCursorTo(24,5);
			printf("COORDS ?\n");
			cursor_pos=input();
			moveCursorTo(24,6);
			printf("               ");
			x=cursor_pos.x;
			y=cursor_pos.y;
			moveCursorTo(24,5);
			printf("ORIENT? H ou V\n");
			r=input_orientation();
			moveCursorTo(24,5);
			printf("              ");
			moveCursorTo(24,5);
			printf("ORIENT OK");
			v = verif(J, x, y, r, bateaux[i]);
			
		}
		moveCursorTo(24,6);
		printf("              ");
		
		if ( r==0) { /* HOrizontal */
			for (int j=0; j<bateaux[i]; j++) {
				J->board[x+j][y] = 'b';
			}
		} else { /*Vertical */
			for (int j=0; j<bateaux[i]; j++) {
				J->board[x][y+j] = 'b';
			}	
	    	}
	    	grille(J1.board,1);
	    
		v = 0;		
   }
   placement=1;
    moveCursorTo(24,4);
    printf("                ");
   moveCursorTo(24,5);
    printf("               ");
    moveCursorTo(24,6);
  printf("               ");
   
}

//permet de vérifier les conditions de placement des bateaux
int verif(joueur* J, int x, int y, int r, int taille) {
	int res = 1;
	if ( (x <0 )|| (y<0) || ( r == 0 && x+taille>BOARD_SIZE) || ( r == 1 && y+taille>BOARD_SIZE )) res = 0;
	if (r == 0) {
		for (int i=x; i<x+taille; i++) {
			if (J->board[i][y] == 'b') res = 0;
		}
	} else {
		for (int i=y; i<y+taille; i++) {
			if (J->board[x][i] == 'b') res = 0;
		}
	}
	if (res == 0) {
	moveCursorTo(24,6);
	printf("             ");
	moveCursorTo(24,6);
	printf("non valide\n");
	
	}
	return res;
}

//cette fonction permet d'afficher les grilles et de jouer les coups
int playGame(joueur * J1){
	//Conditions de victoire 
	int x,y;
	while (J1->vie >0 || J1->vie_a >0){
	
		if (tour == 1) {
			//Affichage des grilles
			moveCursorTo(24,2);
			printf("              ");
			moveCursorTo(24,2);
			printf("GRILLE DE JEU");
			grille(J1->vierge,1);
			
			moveCursorTo(24,4);
			printf("                 ");
			moveCursorTo(24,4);
			printf("A TOI DE JOUER");
			//Selection de la cible 
			cursor_pos=input();
			x=cursor_pos.x;
			y=cursor_pos.y;
			
			sprintf(buffer,"%d%d",x,y);
			send_lora(dev, buffer);
			read_lora(buffer);
			
			/* 
			LoRa -> envoi des coordonnées à l'autre mintel
			L'autre minitel renvoie plouf ou boom
			
			LoRa -> reception du message plouf ou boom
			
			*/
			
			if (strcmp(buffer,"BOOM")==0){
				moveCursorTo(24,7);
				printf("BOOM \n");
				J1->vie_a--;
				J1->vierge[x][y] = 'x';
	   		} 
	   		else {
	   			moveCursorTo(24,7);
				printf("PLOUF\n");
				J1->vierge[x][y] = 'o';
				}
			grille(J1->vierge,1);
			tour *= -1;
		}
		// TOUR ADVERSE
		else{
		
		moveCursorTo(24,2);
		printf("              ");
		moveCursorTo(24,2);
		printf("VOTRE GRILLE");
		moveCursorTo(24,4);
		printf("              ");
		moveCursorTo(24,4);
		printf("L ADVERSAIRE JOUE");
		grille(J1->board,1);
		
		//const char * lora_recu="45";  // x: 4, y: 5
		read_lora(buffer);
		int selection = atoi(buffer);
		y=selection % 10;
		x=selection / 10;
		if (J1->board[x][y] == 'b'){
			send_lora(dev, "BOOM");
			/*
			LoRa -> reception de la case choisie
			-> Retour en fonction de si ça touche ou coule
			const retour="BOOM
			*/
			moveCursorTo(24,7);
			printf("BOOM \n");
			J1->vie--;
			J1->board[x][y] = 'x';
		}
		else {
			send_lora(dev, "PLOUF");
			moveCursorTo(24,7);
			printf("PLOUF \n");
			J1->board[x][y] = 'o';
		}
		
		tour *= -1;
		
		}
		
		moveCursorTo(34,18);
		printf("  ");
		moveCursorTo(34,18);
		printf("%d", J1->vie);
		
		moveCursorTo(31,20);
		printf("  ");
	        moveCursorTo(31,20);
		printf("%d", J1->vie_a);
		grille(J1->board,1);
	}
	
	moveCursorTo(24,8);
	printf("Partie FINIE");
	return 1;
}

void navaleMain(void){
    
    dev = setup_lora(125,12,5,868000000);
	if(dev==NULL){
		return;
    }
    enableScreen(0);
    clearScreen(); 
    
    courant=1;
    J1.vie = 17;
    J1.vie_a = 17;
    
    xtimer_sleep(5);
    
        moveCursorTo(5,4);
    printf("CONFIGURATION SANS FIL...");

clearScreen(); 
   // send_lora(dev, "Hello World!");
    moveCursorTo(22,12);
    printf("ZQSD : BOUGER");
    
     moveCursorTo(22,14);
    printf("ENTRER : VALIDER");
    
     moveCursorTo(22,16);
    printf("* : CHANGER GRILLE");
    
     moveCursorTo(22,18);
    printf("VOTRE VIE : %d", J1.vie);
    
     moveCursorTo(22,20);
    printf("VIE J2 : %d", J1.vie_a);
    
    moveCursorTo(24,4);
    printf("CHARGEMENT");
    initBoard(&J1); 
    grille(J1.board,0); // 0 -> initialisation de la grille
    if (MINITEL_NUM == 2){
    	tour=-1;
    }
    if (tour==1){
    	 placeShips(&J1);		
    	send_lora(dev, "START");
    	
    	moveCursorTo(24,4);
    	printf("             ");
    	moveCursorTo(24,4);
	printf("J2 PLACE\n");
	moveCursorTo(24,5);
    	printf("             ");
    	
    	read_lora(buffer);
    }
    else {
    	moveCursorTo(24,4);
    	printf("             ");
    	moveCursorTo(24,4);
	printf("J1 PLACE\n");
    	read_lora(buffer);
    	placeShips(&J1);
    	send_lora(dev, "START");
    }
    playGame(&J1);
}
/*

================================================ UTILS =============================================

*/

void grille(char jo[10][10], int mode){
	if (mode==0){
		/* INDICES HORIZONTAL*/
		moveCursorTo(2,2);
		for(int i=0; i<10; i++){
			printf("%d ",i);	
		}
		
		/* INDICES VERTICAL*/
		moveCursorTo(1,3);
		for(int i=0; i<10; i++){
			printf("%d",i);	
			moveCursor(LEFT,1);
			moveCursor(DOWN,2);
		}
	}
	
	
	moveCursorTo(2,3);
	/* AFFICHAGE CONTENU*/
	for(int j=0; j<10; j++){
		for(int i=0; i<10; i++){
			printf("%c",jo[i][j]);
			moveCursor(RIGHT,1);
		}
		moveCursor(DOWN,2);
		moveCursor(LEFT,20);
	}
}


Cursor input(void)
{
    Cursor prev_pos;
    /* Local variable containing the message exchanged on UART interface between the STM32 and the Minitel */
    msg_t msg;
    /* Variable containing the input character */
    char buf = 0;
    fflush(stdout);
    cursor_pos.x = 5;
    cursor_pos.y=5;
    moveCursorTo(2+2*5, 3+2*5);
    prev_pos = cursor_pos;
    specialChar(SPE_CHAR_HASHTAG);
    int flag_enter=0;
    while (!flag_enter){
	    while (buf != '\r')
	    {
		msg_receive(&msg);
		buf = (char)msg.content.value;
		if (buf != 0)
		{
			    prev_pos = cursor_pos;
			    moveCursorTo(2+2*prev_pos.x, 3+2*prev_pos.y);
			    if (placement==0){
			    printf("%c",J1.board[prev_pos.x][prev_pos.y]);
			    }
			    
			    else{
			    printf("%c",J1.vierge[prev_pos.x][prev_pos.y]);
			    }
			    
			    if (buf == '*'){
			    	if (courant == 1) {
			    		 grille(J1.board,1);
			    		 moveCursorTo(24,2);
			    		 printf("               ");
			    		 moveCursorTo(24,2);
			    		 printf("VOTRE GRILLE");
			    		 courant = 2;
			    	}
			        else {
			        	grille(J1.vierge,1);
			        	 moveCursorTo(24,2);
			    		 printf("               ");
			        	moveCursorTo(24,2);
			        	printf("GRILLE DE JEU");
			        	courant = 1;
			        }
			    }
			    else if (buf == 'Q' || buf == 'q'){
			    cursor_pos = move(cursor_pos, 0);
			    moveCursorTo(3,24);
			    printf("         ");
			    moveCursorTo(3,24);
			    printf("(%d,%d)",cursor_pos.x,cursor_pos.y);
			    }
			    else if (buf == 'Z' || buf == 'z'){
			    cursor_pos = move(cursor_pos, 1);
			    moveCursorTo(3,24);
			    printf("         ");
			    moveCursorTo(3,24);
			    printf("(%d,%d)",cursor_pos.x,cursor_pos.y);
			    }
			    else if (buf == 'D' || buf == 'd'){
			    
			    cursor_pos = move(cursor_pos, 2);
			    moveCursorTo(3,24);
			    printf("         ");
			    moveCursorTo(3,24);
			    printf("(%d,%d)",cursor_pos.x,cursor_pos.y);
			    }
			    else if (buf == 'S' || buf == 's'){
			    
			    cursor_pos = move(cursor_pos, 3);
			    moveCursorTo(3,24);
			    printf("         ");
			    moveCursorTo(3,24);
			    printf("(%d,%d)",cursor_pos.x,cursor_pos.y);
			    }
			    
			    // ENTER
			    else if (buf == 13) {
			    moveCursorTo(3,24);
			    printf("           ");
			    moveCursorTo(3,24);
			    printf("ENTRER");
			    flag_enter=1;
			    
			    }
			    moveCursorTo(2+2*cursor_pos.x, 3+2*cursor_pos.y);
			    specialChar(SPE_CHAR_HASHTAG);
			    
			    fflush(stdout);
			    break;
		 }
	    }
	}
	return cursor_pos;
}


int input_orientation(void)
{
    /* Local variable containing the message exchanged on UART interface between the STM32 and the Minitel */
    msg_t msg;
    /* Variable containing the input character */
    char buf = 0;
    fflush(stdout);
    int flag_enter=-1;
    while (flag_enter<0){
		msg_receive(&msg);
		buf = (char)msg.content.value;
		if (buf != 0)
		{
			    if ((buf == 'H') || (buf =='h')) { flag_enter=0;
			    }
			    if ((buf == 'V') || (buf == 'v')) { flag_enter=1;
				 }
		}
	}
    return flag_enter;
}		

/*
Sequences pour l'echo
 True: [PRO3, AIGUILLAGE_ON, RCPT_ECRAN, EMET_MODEM],
            False: [PRO3, AIGUILLAGE_OFF, RCPT_ECRAN, EMET_MODEM]
            
*/

void enableScreen(int val){
	if (val == 0){
	
	printf("%c", 0x1b);
	printf("%c", 0x3b);
	printf("%c", 0x60);
	printf("%c", 0x58);
	printf("%c", 0x52);
	}
	else {
	printf("%c", 0x1b);
	printf("%c", 0x3b);
	printf("%c", 0x61);
	printf("%c", 0x58);
	printf("%c", 0x52);
	}	
}


void filltab(char tab[10][10], char c){
	for(int j=0; j<10; j++){
		for(int i=0; i<10; i++){
			tab[i][j]=c;
		}
	}
}

/*
Q -> 0 // GAUCHE
Z -> 1 // HAUT
D -> 2 // DROITE
S -> 1 // BAS


*/

Cursor move(Cursor curr_pos, int dir){
	Cursor copy;
	copy.x=curr_pos.x;
	copy.y=curr_pos.y;
	switch(dir){
		case 0:
			copy.x-=1;
			break;
		case 1:
			copy.y-=1;
			break;	
		case 2:
			copy.x+=1;
			break;
		default:
			copy.y+=1;
	}
	if (copy.x < 0 || copy.x > 9 || copy.y < 0 || copy.y > 9) return curr_pos;
	return copy;
}


void read_lora(char* msg){
	while(flag_rx_lora!=1);
	sprintf(msg,"%s",message);
	flag_rx_lora=0;
}