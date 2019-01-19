/**
 * This file provides the control for the the opponent.
 * @file computer.c
 * @author Ryan Petter
 * @date 10/26/2018
 */
#define OPPONENT_C

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include "computer.h"
#include "pong.h"

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

/************************************************************************************
 * Constant declarations / table declarations
 ***********************************************************************************/
#define PADDLE_SIZE (5)


/************************************************************************************
 * Method header:
 * Function name: moveopponent
 * Function purpose: This function is responsible for moving the opponents paddle.
 *                   It is spawned as a thread and will exit if and when quit is no longer true.
 * Function parameters: 
 *                   void *vp - This is a pointer to the parameters passed into the 
 *                              thread.  At the present time, this parameter is not used.
 * Function return value: void* This is the return value when the thread exits.  
 *                              Currently, it is always NULL, as no data is directly 
 *                              returned by the thread.
 ************************************************************************************/
void *moveopponent(void* vp) {


	srand(time(NULL));

	// get the extents of the screen
	int maxx;
	int maxy;
	getmaxyx(win,maxy,maxx);

	maxy -= 2;


	int previous_ballx = 0;
	int previous_bally = 0;
	opponentVPos = maxy/2;
	int r = 0;

	//lock display to draw paddle
	pthread_mutex_lock(&display);

	// draw the default paddle
	for (int i=0;i<maxy;i++) {

		if(i >= opponentVPos && i < opponentVPos+PADDLE_SIZE) {
			mvaddch(i,maxx-1,' ' | A_REVERSE);
		}else {
			mvaddch(i,maxx-1,' ' | A_NORMAL);
		}

	}

	//unlock display
	pthread_mutex_unlock(&display);

	//while the game is running 
	while (!quit) {

		//while the game is running but paused do nothing
		while(pauseGame && !quit);

		//lock display
		pthread_mutex_lock(&display);

		//generate random number within range of display length divided by 5
		r = rand() % maxy/5;

		//(ball going down or ball is much further down than paddle) and ball is heading toward computer paddle and ball is further away then 
		//display length divided by 10
		if((previous_bally < bally-1  || bally > opponentVPos+PADDLE_SIZE+maxy/10) && previous_ballx < ballx && bally > opponentVPos-maxy/10) {

			//for random number generated
			for(int i = r; i > 0; i--) {

				//increment opponent paddle position
				opponentVPos++;

				if(opponentVPos > maxy-5) {
					opponentVPos = maxy-5;
				}

				//redraw paddle
				mvaddch(opponentVPos+PADDLE_SIZE-1,maxx-1,' ' | A_REVERSE);
				mvaddch(opponentVPos-1,maxx-1,' ' | A_NORMAL);

				//update display
				refresh();

				//sleep to allow smoother paddle movement
				usleep(10000);
			}

			//save y positon of ball
			previous_bally = bally;
			
		//(ball going up or ball is much further up than paddle) and ball is heading toward computer paddle and ball is further away then 
		//display length divided by 10
		}else if((previous_bally > bally+1 || bally < opponentVPos-maxy/10) && previous_ballx < ballx   && bally < opponentVPos+PADDLE_SIZE+maxy/10){

			//for random number generated
			for(int i = r; i > 0; i--) {

				//decrement opponent position
				opponentVPos--;

				if(opponentVPos < 0) {
					opponentVPos = 0;
				}
				
				//redraw paddle at new location
				mvaddch(opponentVPos,maxx-1,' ' | A_REVERSE);
				mvaddch(opponentVPos+PADDLE_SIZE,maxx-1,' ' | A_NORMAL);

				//update display
				refresh();

				//sleep to allow smoother paddle movement
				usleep(10000);

			}

			//save y positon of ball
			previous_bally = bally;
		}

		//save x positon of ball
		previous_ballx = ballx;

		//unlock display
		pthread_mutex_unlock(&display);

		//sleep to not kill cpu
		usleep(ballSpeed/2);

	}
	return NULL;
}

