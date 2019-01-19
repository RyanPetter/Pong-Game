/**
 * This file provides the control for the ball. It is implemented in a seperate thread
 * that is created from the main file.
 * @file ballctrl.c
 * @author Ryan Petter
 * @date 10/17/2018
 */
#define BALLCTRL_C
#define FLASH_COUNT (10)

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include <unistd.h>
#include <stdbool.h>
#include "ballctrl.h"
#include "pong.h"

/************************************************************************************
 * Private structure / type definitions
 ************************************************************************************/
enum ballDirection{
	RIGHT,
	LEFT
};

enum Side{
	USER,
	OPPONENT,
	MIDDLE
};

enum CollisionType{
	WALL,
	PADDLE
};

struct Ball{
	float xCoor;
	float yCoor;
	float xMovement;
	float yMovement;
	enum ballDirection direction;
	int flashCount;
	uint32_t* speed;
};
/************************************************************************************
 * Private function / method prototypes
 ************************************************************************************/
static void collision(enum CollisionType type, enum Side side);
static void initBall(struct Ball* ball, enum ballDirection direction,int maxx);
static void resetBall(struct Ball* ball, enum ballDirection direction, int maxx);
static void flashBall(struct Ball* ball);
/************************************************************************************
 * Constant declarations / table declarations
 ***********************************************************************************/



/**
 * This is the method for main thread execution. It will control the ball and take actions
 * when necessary. This function will access the ball position and collision global data
 * variables to communicate with other processes. This function will continue execution
 * until quit has been signaled by another process.
 * @param vp	not currently used.
 */
void *moveball(void* vp) {
	int maxx;
	int maxy;

	getmaxyx(win,maxy,maxx);
	maxy -= 2;

	struct Ball ball;
	initBall(&ball, LEFT, maxx);

    while(!quit) {

    	while(pauseGame && !quit);

    	pthread_mutex_lock(&display);
		mvaddch(bally,ballx,' ' | A_NORMAL);

		//adjust ball coordinates
		ball.yCoor+=ball.yMovement;
		ball.xCoor+=ball.xMovement;
		
		// truncate and save to global memory
		bally = (int)(ball.yCoor);
		ballx = (int)(ball.xCoor);

		//check for collision with top of wall
		if (bally>maxy-2)
		{
			ball.yMovement = -ball.yMovement;
			collision(WALL,MIDDLE);
		}
		if (bally < 1)
		{
			ball.yMovement = -ball.yMovement;
			collision(WALL,MIDDLE);
		}


		//x-direction collision check. This must check whether a ball has collided with the call
		//or if a ball has collided with a paddle.
		if(ball.direction == RIGHT){
			//check if the ball has collided with the right paddle
			if(ballx == maxx-2 && bally <= (opponentVPos + 5) && bally >= opponentVPos){
				ball.direction = LEFT;
				ball.xMovement = -ball.xMovement;
				collision(PADDLE,OPPONENT);
			}else if(ballx > maxx-1){
				collision(WALL,OPPONENT);
				flashBall(&ball);
				resetBall(&ball,LEFT,maxx);
			}
		}else{
			//ball is moving left. Check if ball has collided with the left wall or paddle
			if(ballx == 1 && bally <= (userVPos + 5) && bally >= userVPos){
				ball.direction = RIGHT;
				ball.xMovement = -ball.xMovement;
				collision(PADDLE,USER);
			}else if(ballx < 1){
				collision(WALL,USER);
				flashBall(&ball);
				resetBall(&ball, RIGHT, maxx);
			}
		}

		mvaddch(bally,ballx,' ' | A_REVERSE );

		pthread_mutex_unlock(&display);

		usleep(ballSpeed);
	}
    return NULL;
}
/**
 * This function updates the score for the correct player and signals the audio controller to play
 * a sound.
 * @param type	the type of collision that has occurred
 * @param side	the side that the collision occurred on
 */
static void collision(enum CollisionType type, enum Side side) {
	if(type == WALL){
		if(side == USER){
			computerScore++;
		}else if(side == OPPONENT){
			playerScore++;
		}
	}

	collisionSound = true;
	return;
}

/**
 * This function is used to initialize an empty ball structure.
 * @param ball			a non null pointer to a ball struct
 * @param direction		the initial direction of the ball
 * @param maxx			the max x coordinate of the screen
 */
static void initBall(struct Ball* ball, enum ballDirection direction, int maxx) {
	if (ball != NULL) {
		//set initial ball speed
		ballSpeed = 100000;
		if (direction == RIGHT) {
			*ball = (struct Ball ) { 1.0f, 1.0f, 1.0f, 1.0f, direction,
							FLASH_COUNT, &ballSpeed };
		} else {
			*ball = (struct Ball ) { (float) (maxx - 2), 1.0f, -1.0f, -1.0f,
							direction, FLASH_COUNT, &ballSpeed };
		}
	}
}

/**
 * This function will execute the animation for flashing the ball upon contact with the left
 * or right wall. The ball will flash at a rate based on the speed of the ball, and will
 * flash a number of times depending on a the flash count set in the ball variable.
 * @param ball
 */
static void flashBall(struct Ball* ball) {
	if (ball != NULL) {
		while (ball->flashCount > 0) {
			if (ball->flashCount % 2 == 0) {
				mvaddch(bally, ballx, ' ' | A_REVERSE);
			} else {
				mvaddch(bally, ballx, ' ' | A_NORMAL);
			}
			ball->flashCount--;
			refresh();
			usleep(ballSpeed);
		}
	}
}

/**
 * This function will reset the ball to it's original state on a side determined by
 * the direction of the ball.
 * @param ball			non-null pointer to a ball structure
 * @param direction		the direction of to send the ball
 * @param maxx			the maximum x coordinate
 */
static void resetBall(struct Ball* ball, enum ballDirection direction, int maxx) {
	if (ball != NULL) {
		ballSpeed = 100000;
		ball->flashCount = FLASH_COUNT;
		if (direction == RIGHT) {
			ball->direction = RIGHT;
			ball->xCoor = 1.0f;
			ball->yCoor = 1.0f;
			ball->xMovement = 1.0f;
			ball->yMovement = 1.0f;
		} else {
			ball->direction = LEFT;
			ball->xCoor = (float) (maxx - 2);
			ball->yCoor = 1.0f;
			ball->xMovement = -1.0f;
			ball->yMovement = -1.0f;
		}
	}
}
