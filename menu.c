#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"


void startMenu() {
	struct user firstUser;
	int highScore;
	///\brief lettura dello highscore del giocatore
	FILE *highScoreFile;
	highScoreFile = fopen("./highScore.txt", "r");
	fscanf(highScoreFile, "%d", &highScore);
	fclose(highScoreFile);
	int maxX = getmaxx(stdscr)/2;
	int maxY = getmaxy(stdscr)/2;
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	attron(COLOR_PAIR(3));
	showTrex(maxY, maxX);
	attroff(COLOR_PAIR(3));
	///\brief Prende le informazioni date nel menu dal giocatore
	mvprintw(maxY+1, maxX-28, "Write inputs and press Enter to start Game.");
    mvprintw(maxY+2, maxX-26, "When you had prize, fire it with 'k' key!");
	mvprintw(maxY+3, maxX-21, "You can jump with space key!");
	mvprintw(maxY+4, maxX-15, "Name: ");
	getstr(firstUser.name);
	mvprintw(maxY+5, maxX-15, "Last name: ");
	getstr(firstUser.lastName);
	mvprintw(maxY+6, maxX-15, "Age: ");
	getstr(firstUser.age);
	noecho();
	startEngine(highScore, firstUser);
	///\brief Istruzioni di stampa per la guida nel menu del gioco, specificando dove inserire i dati e come giocare.
}

/**
\fn void startMenu ()
\biref Procedura che svolge le istruzioni del menu, di apertura del gioco, dell'inserimento e racoolta dati del giocatore e del successivo avvio del gioco.
*/

void endGame(int score, int highScore, int diY, int diX, struct user firstUser) {
	nodelay(stdscr, FALSE);
	init_pair(2,COLOR_RED,COLOR_BLACK);
	///\brief Riga 47 chiamata procedura per il salvataggio del gioco
	if (score > highScore) {
		highScore = score;
		FILE *highScoreFile;
		highScoreFile = fopen("./highScore.txt", "w");
		fprintf(highScoreFile, "%d", highScore);
		fclose(highScoreFile);
	}
	/**
	\brief Da riga 49 a 54 viene verificato se lo score effettuato in partita e' piu' alto dell'highscore, in tal caso sostituisce l'highscore precedente con quello nuovo
	*/
	int maxX = getmaxx(stdscr)/2;
	int maxY = getmaxy(stdscr)/2;
	attron(COLOR_PAIR(2));
	showLoss(maxY, maxX);
	mvprintw(diY-4, diX, "          X-X ");
	mvprintw(diY, diX, "      ||");
	char keyToExit = getch();
	if (keyToExit == 'r') {
		attroff(COLOR_PAIR(2));
		startEngine(highScore, firstUser);
	}
	else if (keyToExit == 'q') {
		return;
	}
	else {
		endGame(score, highScore, diY, diX, firstUser);
	}
	///\brief La riga 65 prende l'input del giocatore, in base se vuole uscire o no dal gioco, e lo esegue.
}

/**
\fn void endGame(int score, int highScore, int diY, int diX, struct user firstUser)
\brief Procedura che esegue la fase di terminazione del gioco, quella di game over
\param int score: memorizza e stampa successivamente a video lo score effettuato dal giocatore
\param int highScore: Legge e stampa l'highscore attuale
\param int diY: prende il punto d'altezza dove il dinosauro e' stato colpito dall' ostacolo e lo ferma
\param int diX: stessa procedura della variabile diY ma fatto orizzontalmente.
*/
