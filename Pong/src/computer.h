/**
 * This file provides the interface for opponent control
 * @file computer.h
 * @author Ryan Petter
 * @date 10/26/2018
 */
#ifndef OPPONENT_H
#define OPPONENT_H

#ifdef OPPONENT_C
#define EXTERN_PFX 
#else
#define EXTERN_PFX extern
#endif

/************************************************************************************
 * Public function / method prototypes
 ************************************************************************************/

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
EXTERN_PFX void *moveopponent(void* vp);
#undef EXTERN_PFX
#endif
