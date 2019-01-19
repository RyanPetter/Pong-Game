/**
 * This file provides the control a player or user.
 * @filename player.c
 * @author Ryan Petter
 * @date 10/18/2018
 */
#define PADDLECTRL_C

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include <curses.h>
#include <unistd.h>
#include "pong.h"


/************************************************************************************
 * Private structure / type definitions
 ************************************************************************************/


/************************************************************************************
 * Private function / method prototypes
 ************************************************************************************/
static void adjustGame(int ch);
static void adjustPlayerPaddle(int ch, int maxy);
static void adjustComputerPaddle(int maxy, int previous_bally);

/************************************************************************************
 * Constant declarations / table declarations
 ***********************************************************************************/
#define PADDLE_SIZE (5)

/************************************************************************************
 * Method header:
 * Function name: moveme
 * Function purpose: This function is responsible for moving the users paddle.
 *                   It is spawned as a thread and ewill exit if and when quit is no longer true.
 * Function parameters: 
 *                   void *vp - This is a pointer to the parameters passed into the 
 *                              thread.  At the present time, this parameter is not used.
 * Function return value: void* This is the return value when the thread exits.  
 *                              Currently, it is always NULL, as no data is directly 
 *                              returned by the thread.
 ************************************************************************************/
// Run the user's paddle
void *moveme(void* vp)
{
	int ch;
	int previous_bally = 0;

	// get the extents of the screen
	int maxx;
    int maxy;
	getmaxyx(win,maxy,maxx);
	maxy -= 2;

	while(!quit) {

		//apply mutex lock (disply is changed here)
		pthread_mutex_lock(&display);

		//get key press
		ch = getch();

		//chagne game state based on key
		adjustGame(ch);
		
		//if single player adjust paddle position based on user
		if(mode == SINGLE_PLAYER){
			adjustPlayerPaddle(ch, maxy);
		//else adjust based on computer algorithm	
		}else{
			adjustComputerPaddle(maxy, previous_bally);
		}

		//save previous ball state
		previous_bally = bally;

		//redraw paddle at userVPos location
		for (int i=0;i<maxy;i++) {
			if(i >= userVPos && i < userVPos+PADDLE_SIZE) {
				mvaddch(i,0,' ' | A_REVERSE);
			}else {
				mvaddch(i,0,' ' | A_NORMAL);
			}
		}

		//unlock mutex
		pthread_mutex_unlock(&display);

		//dont kill the cpu
		usleep(10000);

	}
	return NULL;
}

/**
 * Updates the paddle position based on computer algorithm. Paddle is made to follow ball exactly and never miss
 **/
static void adjustComputerPaddle(int maxy,int previous_bally) {
	//if ball has moved and is in range to follow
	if (bally != previous_bally && bally <= maxy - 3 && bally >= 2) {

		//set global variable for opponent paddle position
		userVPos = bally - 2;

	}
	previous_bally = bally;
}

/**
 * Adjusts paddle position based on user input. Down if user hit down arrow and up if user hit up arrow
 **/
static void adjustPlayerPaddle(int ch, int maxy){
	switch (ch) {

	case KEY_UP:
		if (!pauseGame) {
			//update user paddle position
			userVPos--;

			if (userVPos < 0) {
				userVPos = 0;
			}
		}
		break;

	case KEY_DOWN:
		if (!pauseGame) {
			//update user paddle position
			userVPos++;

			if (userVPos > (maxy - PADDLE_SIZE)) {
				userVPos = (maxy - PADDLE_SIZE);
			}
		}
		break;
	}
}

/**
 * Changes game state based on input. q to quit game. p to pause game.
 * Left/Right arrow to adjust ball speed. -/= to adjust game volume
 **/
static void adjustGame(int ch) {
	switch (ch) {
	case 'q':
		
		//quite game 
		quit = true;
		break;

	case 'p':
		//pause or unpause game
		if (pauseGame) {
			pauseGame = false;
		} else {
			pauseGame = true;
		}
		break;

	case KEY_LEFT:

		//decrease ball speed by factor of 1.25
		if (!pauseGame && ballSpeed < 100000) {
			ballSpeed *= 1.25;
		}
		break;

	case KEY_RIGHT:
		
		//increase ball speed by factor of 1.25
		if (!pauseGame && ballSpeed > 20000) {
			ballSpeed /= 1.25;
		}
		break;

	case '-':
		
		//decrease sound down to -4
		gain--;

		if (gain < -4) {
			gain = -4;
		}
		break;

	case '=':
		
		//increase sound up to 2
		gain++;

		if (gain > 2) {
			gain = 2;
		}
		break;

	default:
		break;
	}
}

