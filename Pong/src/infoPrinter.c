/**
 * This file provides the logic for the printing info to the screen. This thread will keep track of
 * time and continue to run until a user quits.
 * @file infoPrinter.c
 * @author Ryan Petter
 * @date 10/19/2018
 */


#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "pong.h"

/**
 * This function is the main thread execution for printing information to the screen. This includes tracking and
 * printing the running time, and printing the scores for both players. This will change based on the mode
 * of the application. This thread will exit when a user quits. This function will access the global
 * player scores.
 * @param vp	This should be the command line arguments
 */
void *infoPrinter(void* vp) {

	// get the extents of the screen
	int maxx;
	int maxy;
	getmaxyx(win,maxy,maxx);

	time_t startTime = time(0);

	int elapsedSeconds = 0;
	int pauseTime = 0;
	int timeSpentPaused = 0;

	pthread_mutex_lock(&display);

	for(int i = maxx-1; i >= 0; i--) {
		mvprintw(maxy-2, i, "_");
	}

	pthread_mutex_unlock(&display);

	while(!quit) {

		if(pauseGame) {

			pauseTime = time(0);

			while(pauseGame && !quit) {
				timeSpentPaused = time(0)-pauseTime;
			}

			startTime += timeSpentPaused;
		}

		pthread_mutex_lock(&display);

		//determine the seconds since program initialization
		elapsedSeconds = time(0) - startTime;

		//cast the function argument so that data can be extracted
		char** argv = (char**)vp;

		//Print player names and time to the screen. Player names are dependent on application mode
		if(mode == NO_PLAYER){
			mvprintw(maxy-1, 0, "Computer1: %d", playerScore);
		}else{
			mvprintw(maxy-1, 0, "%s: %d", argv[2], playerScore);
		}
		mvprintw(maxy-1, maxx/2-17, "Time Elapsed: %d minutes %d seconds ", elapsedSeconds/60, elapsedSeconds%60);

		if(mode == NO_PLAYER){
			mvprintw(maxy-1, maxx-12, "Computer2: %d", computerScore);
		}else{
			mvprintw(maxy-1, maxx-12, "Computer: %d", computerScore);
		}

		pthread_mutex_unlock(&display);

		usleep(500000);
	}

	return NULL;

}

