/**
 * This file provides the entry point for pong. It will do the proper initialization
 * and create the necessary threads.
 * @file Pong.c
 * @author Ryan Petter
 * @date 10/16/2018
 */
/************************************************************************************/
#define PONG_C

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include <pthread.h>
#include <curses.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#include "pong.h"
#include "player.h"
#include "ballctrl.h"
#include "computer.h"
#include "infoPrinter.h"
#include "audioController.h"

static void initGlobalData();
static void initWindow();
static void initThreads(pthread_t* threads, int* remoteCalls, char** argv);
static void joinThreads(pthread_t* threads);
static void tearDownWindow();

/************************************************************************************
 * Method header:
 * Function name: main
 * Function purpose: This is the main function for the program. It spawns 5 processes
 * 					 to handle the execution of the game of pong.
 * Function parameters: 
 *                   int argc  - This is a count of the number of command line parameters 
 *                               passed by the operating system.
 *                   char *vp[] - This is the array of strings which is passed to program.
 * Function return value: int.  0 will be returned upon normal exiting of the program.  
 *                                A negative value will indicate an error.
 ************************************************************************************/
int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		printf("For single player, please enter an audio output device (plughw:0) followed by a player name."
				"\nFor CPU vs CPU, please enter the audio output device (plughw:0) only.\n");
		return 0;
	}else if(argc == 2){
		mode = NO_PLAYER;
	}else{
		mode = SINGLE_PLAYER;
	}

	pthread_t threads[5];
	int remoteCalls[5];

	initGlobalData();
	initWindow();
	initThreads(&threads[0], &remoteCalls[0], &argv[0]);
	joinThreads(&threads[0]);
	tearDownWindow();
	
	return 0;
}

/**
 * This function initializes all global data to an initial state so the program works
 * correctly from the start.
 */
static void initGlobalData(){
	// Initialize all of the variables.
	// Global data - for inter-thread communication
	ballx = 1;
	bally = 1;
	quit = false;
	userVPos = 0;
	gain = 0;
}

/**
 * This is a helper function which initalizes the curses window.
 */
static void initWindow(){
	// init window - see curses documentation for guidance
	win = initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(win,TRUE);
	nodelay(win,TRUE);	//This causes the getch() to be non-blocking
	refresh();
}

/**
 * This is a helper function which handles tearing down the curses window.
 */
static void tearDownWindow(){
	// tear down the window
	delwin(win);
	endwin();
	refresh();
}

/**
 * This is a helper function which accepts and initializes all threads in an array of threads which
 * has a length of 5. It is the caller's responsibility to pass 5 threads in.
 * @param threads		a non null array of threads to initialize.
 * @param remoteCalls	a non null array which will store the results of thread creation
 * @param argv			arguments passed in from the command line.
 */
static void initThreads(pthread_t* threads, int* remoteCalls, char** argv) {
	if (threads != NULL && remoteCalls != NULL && argv != NULL) {
		// Start the threads
		if ((remoteCalls[0] = pthread_create(&threads[0], NULL, &moveball, NULL))) {
			fprintf(stderr, "Ball movement thread creation failed.");
		}
		if ((remoteCalls[1] = pthread_create(&threads[1], NULL, &moveme, NULL))) {
			fprintf(stderr, "Player thread creation failed");
		}

		if ((remoteCalls[2] = pthread_create(&threads[2], NULL, &playAudio,
				argv[1]))) {
			fprintf(stderr, "Player thread creation failed");
		}
		if ((remoteCalls[3] = pthread_create(&threads[3], NULL, &moveopponent,
				NULL))) {
			fprintf(stderr, "Opponent thread creation failed");
		}

		if ((remoteCalls[4] = pthread_create(&threads[4], NULL, &infoPrinter,
				argv))) {
			fprintf(stderr, "Player thread creation failed");
		}
	}

}

/**
 * This is a helper function which joins all threads so that the main thread suspends until
 * all other threads are finished.
 * @param threads A non-null array of length 5 which holds all threads.
 */
static void joinThreads(pthread_t* threads) {
	if (threads != NULL) {
		// Wait for the threads to exit
		pthread_join(threads[0], NULL);
		pthread_join(threads[1], NULL);
		pthread_join(threads[2], NULL);
		pthread_join(threads[3], NULL);
		pthread_join(threads[4], NULL);

	}
}
