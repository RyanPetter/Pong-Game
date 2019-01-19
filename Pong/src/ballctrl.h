/**
 * This file provides the interface for ball control
 * @file ballctrl.h
 * @author Ryan Petter
 * @date 10/17/2018
 */
#ifndef BALLCTRL_H
#define BALLCTRL_H

#ifdef BALLCTRL_C
#define EXTERN_PFX
#else
#define EXTERN_PFX extern
#endif

/**
 * This is the method for main thread execution. It will control the ball and take actions
 * when necessary. This function will access the ball position and collision global data
 * variables to communicate with other processes. This function will continue execution
 * until quit has been signaled by another process.
 * @param vp	not currently used.
 */
EXTERN_PFX void *moveball(void* vp);

#undef EXTERN_PFX
#endif
