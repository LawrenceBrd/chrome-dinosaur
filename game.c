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

int checkGame(int y, int x, int diY, int diX);
int computeTime(int delayTime);
void showdinosaur(int diY, int diX);
int computePrize(int score, int usedPrize);

int tests_run=0;
int tests_passed=0;

void test_checkGame() {
	int oracolo=0;
	mu_assert("ERRORE in funzione checkGame(), (3+14)-17<=4",checkGame(2,13,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), diY==y",checkGame(2,13,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (4+14)-14<=4",checkGame(2,14,2,4)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (5+14)-15<=4",checkGame(2,15,2,5)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (1000+14)-1014<=4",checkGame(2,1014,2,1000)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (-24+14)-(-10)<=4",checkGame(2,-10,2,-24)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), (5+14)-20<=4",checkGame(2,20,2,5)==oracolo);
	oracolo=1;
	mu_assert("ERRORE in funzione checkGame(), diY!=y",checkGame(224,14,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), diY!=y",checkGame(1,14,2,3)==oracolo);
	mu_assert("ERRORE in funzione checkGame(), diY!=y",checkGame(-56,14,2,3)==oracolo);
}

void test_computeTime() {
	mu_assert("ERRORE in funzione computeTime(), (250000>=250000)",computeTime(250000)==249000);
	mu_assert("ERRORE in funzione computeTime(), (300000>=250000)",computeTime(300000)==299000);
	mu_assert("ERRORE in funzione computeTime(), (1800000>=250000)",computeTime(1800000)==1799000);
	mu_assert("ERRORE in funzione computeTime(), (230000>=200000)",computeTime(230000)==229400);
	mu_assert("ERRORE in funzione computeTime(), (200000>=200000)",computeTime(200000)==199400);
	mu_assert("ERRORE in funzione computeTime(), (249999>=200000)",computeTime(249999)==249399);
	mu_assert("ERRORE in funzione computeTime(), (190000<200000)",computeTime(190000)==189800);
	mu_assert("ERRORE in funzione computeTime(), (80000<200000)",computeTime(180000)==179800);
	mu_assert("ERRORE in funzione computeTime(), (1000<200000)",computeTime(1000)==800);
	mu_assert("ERRORE in funzione computeTime(), (199999<200000)-200",computeTime(199999)==199799);
}

void test_computePrize() {
	mu_assert("ERRORE in funzione computePrize(), score >= 20 && score <= 40 && usedPrize == 0",computePrize(10,0)==0);
	mu_assert("ERRORE in funzione computePrize(), score >= 20 && score <= 40 && usedPrize == 0",computePrize(15,0)==0);
	mu_assert("ERRORE in funzione computePrize(), score >= 20 && score <= 40 && usedPrize == 0",computePrize(30,0)==1);
	mu_assert("ERRORE in funzione computePrize(), score >= 20 && score <= 40 && usedPrize == 0",computePrize(30,1)==0);
	mu_assert("ERRORE in funzione computePrize(), score >= 60 && score <= 80 && usedPrize <= 1",computePrize(70,1)==1);
	mu_assert("ERRORE in funzione computePrize(), score >= 60 && score <= 80 && usedPrize <= 1",computePrize(70,2)==0);
	mu_assert("ERRORE in funzione computePrize(), score >= 100 && score <= 120 && usedPrize <= 2",computePrize(110,2)==1);
	mu_assert("ERRORE in funzione computePrize(), score >= 100 && score <= 120 && usedPrize <= 2",computePrize(130,0)==0);
	mu_assert("ERRORE in funzione computePrize(), score >= 100 && score <= 120 && usedPrize <= 2",computePrize(130,2)==0);
	mu_assert("ERRORE in funzione computePrize(), score >= 20 && score <= 40 && usedPrize == 0",computePrize(10,3)==0);
}

void all_tests() {
	mu_run_test(test_checkGame);
	mu_run_test(test_computeTime);
	mu_run_test(test_computePrize);
}

int run_all_tests(int argc, char **argv) {
	if(argc<2 || strcmp(argv[1],"-test")!=0)
	{
		return -1;
	}
	printf("\n            ESEGUO I TEST\n");
	all_tests();
	printf("\n    --Test eseguiti: %d\n",tests_run);
	printf("\n    --Test eseguiti con successo: %d\n",tests_passed);
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
