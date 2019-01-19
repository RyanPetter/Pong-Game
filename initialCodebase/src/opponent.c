/************************************************************************************
* This file was developed as part of CS3841 Design of Operating Systems at the 
* Milwaukee School of Engineering.  This file is copyright 2008-2014 by MSOE.
* 
* $Author: wws $
* $Revision: 1.1 $
* $Name:  $
* This file manages the motion of the oponent's paddle.
*
************************************************************************************/
#define OPPONENT_C

/************************************************************************************
 * External Includes
 ************************************************************************************/
#include "opponent.h"
#include "pong.h"

/************************************************************************************
 * Private structure / type definitions
 ************************************************************************************/

/************************************************************************************
 * Private function / method prototypes
 ************************************************************************************/


/************************************************************************************
 * Constant declarations / table declarations
 ***********************************************************************************/



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
void *moveopponent(void* vp)
{
  while (!quit)
  {
    // Do something here.
  }
  return NULL;
}

