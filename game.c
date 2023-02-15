#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"
#include "minunit.h"

/**
\file game.c
\version 1.1
\date 2023-01-24
*/

int test_run=0;
int test_passed=0;

void test_checkGame() {
	int oracolo=0;
	mu_assert("ERRORE in funzione checkGame(), (diX+14)-x=4",checkGame(2,13,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), diY==y",checkGame(2,13,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (diX+14)-x<4",checkGame(2,17,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (diX+14)-x<4",checkGame(2,137,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (diX+14)-x<4",checkGame(2,1000,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (diX+14)-x<4",checkGame(2,34,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (diX+14)-x<4",checkGame(2,25,2,3)==oracolo);
	oracolo=1;
	mu_assert("ERRORE in funzione checkGame(), diY!=y",checkGame(224,14,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), diY!=y",checkGame(1,14,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), diY!=y",checkGame(-56,14,2,3)==oracolo);
}

void all_test() {
	mu_run_test(test_checkGame);
}

int run_all_test(int argc, char **argv) {
	if(argc<2 || strcmp(argv[1],"-test")!=0)
	{
		return -1;
	}
	printf("\nESEGUO I TEST\n");
	all_test();
	printf("\nTest eseguiti: %d\n",test_run);
	printf("\nTest eseguiti con successo: %d\n",test_passed);
}

int checkGame(int y, int x, int diY, int diX) {
	if (diY == y) {
		if (abs((diX+14)-x) <= 4) {
			return 0;
		}
	}
	return 1;
}

/**
\fn int checkGame(int y, int x, int diY, int diX)
\brief Funzione che controlla se il gioco deve continuare 
\param int y: 
\param int x:
\param int diY:
\param int diX: 
\return 0: Il gioco si interrompe
\return 1: Il gioco deve continuare
*/

int computeTime(int delayTime) {
	if (delayTime >= 250000) {
		delayTime -= 1000;
	}
	else if (delayTime >= 200000) {
		delayTime -= 600;
	}
	else {
		delayTime -= 200;
	}
	return delayTime;
}

/**
\fn int computeTime(int delayTime)
\brief Funzione che incrementa la velocita' in base a quanto si va avanti
\param delayTime : Valoe di delay attuale passato dal main
\return  delayTime : Delay diminuito ogni volta chiamata funzione  
*/

void showdinosaur(int diY, int diX) {
	static int counter = 0;
	if (counter == 0) {
		dinosaur1(diY, diX);
		counter++;
		///\brief se il contatore e' pari a 0, il dinosauro verra' stampato nella sua posizione principale
	}
	else {
		dinosaur2(diY, diX);
		counter--;
		///\brief se dovesse essere maggiore di 0 o minore, verra' stampato nella sua seconda posizione in corsa
	}
}

/**
\fn void showdinosaur(int diY, int diX)
\brief Procedura che sceglie in quale posizione stampare il dinosauro
\param diY : Parametro per l'altezza (se il dinosauro salta o meno)
\param diX : Parametro dello spostamento in orizzontale del dinosauro
*/


int computePrize(int score, int usedPrize) {
	if (score >= 20 && score <= 40 && usedPrize == 0) {
		return 1;
	}
	else if (score >= 60 && score <= 80 && usedPrize <= 1) {
		return 1;
	}
	else if (score >= 100 && score <= 120 && usedPrize <= 2) {
		return 1;
	}
	///\brief Se lo score e il prize sono compresi entro certi valori, ritornera' 1 e permettera' l'incremento dello score del gioco	
	return 0;
	///\brief Se non dovessero soddisfare i requisiti, ritorna 0 e non esegue l'incremento e l'aumento del prize
}

/**
\fn int computePrize(int score, int usedPrize)
\brief Funzione che tiene conto del punteggio del giocatore
\param score : Contatore del punteggio del giocatore 
\param usedPrize : Variabile da incrementare in base allo score usata per distruggere i cactus
\return 0 : Se i requisiti degli if non vengono soddisfatti, restituito 0
\return 1 : Se li soddisfano, restituito 1 e incrementato lo score
\bug Determinate volte, il punteggio/score del giocatore non viene incrementato e/o delle volte si interrompe nel suo conteggio dei punti
*/


void startEngine(int highScore, struct user firstUser) {
    srand(time(NULL));
	int x, y, diX=5, prize=0, usedPrize=0, score=0, delayTime = 300000
    , gameStatus=1, cactusNum=0;
	int maxX=getmaxx(stdscr);
	x = maxX-20;
	y = getmaxy(stdscr)-6;
	int diY = y;
	int arrowX=(diX+15), arrowY=(diY-3);
	char userInput;
	int jumping=-1;
    bool fire=FALSE;
    ///Tutte le variabili inizializzate utili al gioco
    clear();
	nodelay(stdscr, TRUE);
  	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_GREEN,COLOR_BLACK);
	init_pair(6,COLOR_YELLOW,COLOR_BLACK);
	while (gameStatus == 1) {
		userInput = getch();
		if((score/50)%2 != 0) {
			moon(10, (maxX/2)-10);
		}
		else {
			attron(COLOR_PAIR(1));
			sun(10, (maxX/2)-10);			
		}
		/**
		\brief In base al valore dello score, sceglie se viene stampata la notte o il giorno
		*/
		if (fire) {
			mvprintw(arrowY, arrowX-2, " ");
		}

		score++;
		mvprintw(1, 6, "%s %s %s", firstUser.name, firstUser.lastName, firstUser.age);
		mvprintw(1, getmaxx(stdscr)-9, "%d:%d", highScore, score);
		///\brief Stampa le informazioni di nome, punteggio e highscore
		prize = computePrize(score, usedPrize);
		mvprintw(3, 6, "Prize: %d    ", prize);
		if (prize == 1) {
			if (userInput == 'k') {
				usedPrize++;
				fire = TRUE;
			}
		}
		if (fire) {
			mvprintw(arrowY, arrowX, "*");
			arrowX += 2;
		}
		if ((x+4)-arrowX <= 1 && fire) {
			clearCactus1(y, x-1);
			mvprintw(arrowY, arrowX-2, " ");
			x = getmaxx(stdscr)-20;
			fire = FALSE;
			arrowX = diX+15;
		}
		/**
		\brief In base al prize del giocatore vengono distrutti i cactus
		*/
		box(stdscr, ACS_VLINE, ACS_HLINE);
		cleardinosaurUp(diY, diX);
		if (x <= 7) {
			x = getmaxx(stdscr)-20;
            cactusNum = rand() % 2;
		}
		///\brief cancella la schermata di gioco
		if (userInput == ' ' && jumping<0) {
			diY -= 7;
			jumping = 3;
		}
		showdinosaur(diY, diX);
		if (userInput == ' ') {
			cleardinosaurDown(diY, diX);		
		}
		if (x-diX <= 7) {
			x -= 10;
		}
        if (cactusNum == 0) {
        	cactus1(y, x);
		}
		else {
			cactus2(y, x);
		}
		if (x-diX <= 7) {
			x += 10;
		}
		gameStatus = checkGame(y, x, diY, diX);
		/**
		\brief Da riga 168 a 188 sono presenti gli if per far saltare il nostro dinosauro, solo se l'input del giocatore dovesse essere la barra spaziatrice
		*/
		jumping--;
		if (jumping==0) {
			diY += 7;
		}
		///\brief Riporta il dinosauro a terra
		mvhline(y+1, 1, '-', getmaxx(stdscr)-3);
        refresh();
        clearCactus1(y, x);
		refresh();
        usleep(delayTime);
        x -= 7;
        delayTime = computeTime(delayTime);
		userInput = 'q';
	}

	endGame(score, highScore, diY, diX, firstUser);
	attroff(COLOR_PAIR(1));
	/**
	\brief Chiamate funzioni e opreazioni che svolgono la chiusura del gioco.
	*/
}

/**
\fn void startEngine(int highScore, struct user firstUser)
\brief Procedura principale che svolge tutte le funzioni e procedure del gioco
\param int highScore : 
\param struct user firstUser : 
\bug il bug principale riguarda i salti in ritardo del dinosauro se viene dato in input il carattere ' ' troppo velocemente.
*/
