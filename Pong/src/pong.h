/**
 * This file provides the interface and global variables for Pong
 * @file Pong.h
 * @author Ryan Petter
 * @date 10/16/2018
 */
#ifndef PONG_H
#define PONG_H

#ifdef PONG_C
#define EXTERN_PFX 
#else
#define EXTERN_PFX extern
#endif

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include <curses.h>
#include <pthread.h>

/************************************************************************************
 * Public structure / type definitions
 ************************************************************************************/
enum Mode{
	NO_PLAYER,
	SINGLE_PLAYER,
};
/************************************************************************************
 * Public / global variable definitions
 ************************************************************************************/
// Global data - for inter-thread communication
EXTERN_PFX int ballx;
EXTERN_PFX int bally;
EXTERN_PFX WINDOW *win;	// the curses window
EXTERN_PFX bool quit;  // a flag to stop all threads
EXTERN_PFX bool pauseGame; //a flag to pause all threads
EXTERN_PFX uint32_t ballSpeed;
EXTERN_PFX int userVPos;
EXTERN_PFX int opponentVPos;
pthread_mutex_t display;

bool collisionSound;

enum Mode mode;
int gain;

int playerScore;
int computerScore;

/************************************************************************************
 * Public function / method prototypes
 ************************************************************************************/
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
EXTERN_PFX int main(int argc, char* argv[]);
#undef EXTERN_PFX
#endif
