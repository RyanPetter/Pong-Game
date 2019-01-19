/**
 * This file provides the interface for audio control
 * @file audioController.h
 * @author Ryan Petter
 * @date 10/24/2018
 */

#ifndef SRC_AUDIOCONTROLLER_H_
#define SRC_AUDIOCONTROLLER_H_

/**
 * This function is responsible for playing audio out to an Audio interface. It will play background music and
 * inject beep noises upon collision.
 * @param vp	currently not used.
 */
void *playAudio(void*);

/**
 * Adjusts the volume of sound by multiplying voltage by 2^gain
 */
void* adjustGain(void*, int, int);

/**
 * Adds values of two buffers together
 */
void* combineSounds(void*, void*, int);

#endif /* SRC_AUDIOCONTROLLER_H_ */
