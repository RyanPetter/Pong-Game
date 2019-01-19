/**
 * This file provides the interface for player control
 * @file player.h
 * @author Ryan Petter
 * @date 10/18/2018
 */
#ifndef PADDLECTRL_H
#define PADDLECTRL_H

#ifdef PADDLECTRL_C
#define EXTERN_PFX 
#else
#define EXTERN_PFX extern
#endif

/************************************************************************************
 * Public function / method prototypes
 ************************************************************************************/

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
EXTERN_PFX void *moveme(void* vp);
#undef EXTERN_PFX
#endif
