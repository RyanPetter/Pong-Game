/************************************************************************************
* This file was developed as part of CS3841 Design of Operating Systems at the 
* Milwaukee School of Engineering.  This file is copyright 2008-2014 by MSOE.
* 
* $Author: wws $
* $Revision: 1.1 $
* $Name:  $
* This file manages the motion of the ball.  This includes collisions, edge
* detections, paddle detection, etc.
*
************************************************************************************/
#define BALLCTRL_C

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include <unistd.h>
#include "ballctrl.h"
#include "pong.h"

/************************************************************************************
 * Private structure / type definitions
 ************************************************************************************/

/************************************************************************************
 * Private function / method prototypes
 ************************************************************************************/
static void collision();

/************************************************************************************
 * Constant declarations / table declarations
 ***********************************************************************************/



/************************************************************************************
 * Method header:
 * Function name: moveball
 * Function purpose: This function is responsible for moving the ball within the game of pong.
 *                   It is spawned as a thread and will exit if and when quit is no longer true.
 * Function parameters: 
 *                   void *vp - This is a pointer to the parameters passed into the 
 *                              thread.  At the present time, this parameter is not used.
 * Function return value: void* This is the return value when the thread exits.  
 *                              Currently, it is always NULL, as no data is directly 
 *                              returned by the thread.
 ************************************************************************************/
// This thread is responsible for moving the ball
void *moveball(void* vp)
{
	// get the extents of the screen
        int maxx;
        
        int maxy;
	// how does this work??
	getmaxyx(win,maxy,maxx);
	// these should be floating point to get slopes other than
	// +/- 45 degrees
	float yadder = 1.0f;
	float xadder = 1.0f;
	float xactual = 1.0f;
	float yactual = 1.0f;

    while(!quit)
	{
		mvaddch(bally,ballx,' ' | A_NORMAL);		
		yactual+=yadder;
		xactual+=xadder;
		
		// truncate
		bally = (int)(yactual);
		ballx = (int)(xactual);
		if (bally>maxy-1)
		{
			yadder = -yadder;
			collision();
		}
		if (bally < 1)
		{
			yadder = -yadder;
			collision();
		}
		if (ballx>maxx-1)
		{
			xadder = -xadder;
			collision();
		}
		if (ballx < 1)
		{
			xadder = -xadder;
			collision();
		}
		mvaddch(bally,ballx,' ' | A_REVERSE );

		// Do not want ball to move too fast...		
		usleep(100000);
                // HINT: This really should be a variable, thus allowing you to speed up or slow
                //  down play to increase / decrease level of difficulty.  But you probably do not want it to be a global variable.
	}
    return NULL;
}

/************************************************************************************
 * Method header:
 * Function name: collision
 * Function purpose: This method will be invoked when a ball collision is detected. 
 * Function parameters: 
 *                   None
 * Function return value: None
 ************************************************************************************/
// collision sound effects and other things.
static void collision()
{
	return;
}
