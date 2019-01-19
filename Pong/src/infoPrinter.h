/**
 * This file provides the interface and global variables for infoPrinter
 * @file infoPrinter.h
 * @author Ryan Petter
 * @date 10/19/2018
 */

#ifndef INFOPRINTER_H_
#define INFOPRINTER_H_

#ifdef INFOPRINTER_C
#define EXTERN_PFX
#else
#define EXTERN_PFX extern
#endif

/**
 * This function is the main thread execution for printing information to the screen. This includes tracking and
 * printing the running time, and printing the scores for both players. This will change based on the mode
 * of the application. This thread will exit when a user quits. This function will access the global
 * player scores.
 * @param vp	This should be the command line arguments
 */
void *infoPrinter(void*);

#endif /* SRC_INFOPRINTER_H_ */
