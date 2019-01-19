/**
 * This file provides the control for audio playing.
 * @file audioController.c
 * @author Ryan Petter
 * @date 10/24/2018
 */

#include "AudioInterface.h"
#include "audioController.h"
#include "pong.h"

#define SAMPLING_RATE (11025)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)
#define BUFFER_SIZE (512)

/**
 * This function is responsible for playing audio out to an Audio interface. It will play background music and
 * inject beep noises upon collision.
 * @param vp	currently not used.
 */
void *playAudio(void* vp){

	//size amount of data read from buffer
	int rc;
	
	//buffers for sounds
	char *buffer;
	char *beepBuffer;
	
	//locations of song files
	int songFile = open("Pim Poy Pocket.raw", O_RDONLY);
	int beep = open("Beep2.raw", O_RDONLY);

	// Instantiate and open a new audio interface.
	initializeAudioInterface(vp, SAMPLING_RATE, NUMBER_OF_CHANNELS,
			SND_PCM_STREAM_PLAYBACK);

	// Open the audio interface.
	openAudioInterface();

	// Allocate a buffer so that we can read data in from the file.  Initialize it to all zeros.
	buffer = (char*) malloc(BUFFER_SIZE);
	memset(&buffer[0], 0, BUFFER_SIZE);

	//allocate buffer for beep noise
	beepBuffer = (char*) malloc(BUFFER_SIZE);
	memset(&beepBuffer[0], 0, BUFFER_SIZE);

	//loop until game is quit
	do {
		
		//do nothing during pause
		while(pauseGame && !quit);

		//read music sound file into buffer
		rc = read(songFile, buffer, BUFFER_SIZE);

		//if file has ended restart
		if(rc <= 0) {
			songFile = open("Pim Poy Pocket.raw", O_RDONLY);
		}

		//if collsion has occurred
		if(collisionSound) {

			//read collision sound file
			rc = read(beep, beepBuffer, BUFFER_SIZE);

			//add beep sound to background music
			combineSounds(buffer, beepBuffer, BUFFER_SIZE);

			//if end of beep file restart it 
			if(rc <= 0) {
				beep = open("Beep2.raw", O_RDONLY);
				collisionSound = false;
			}
		}


		//adjust buffer
		adjustGain(buffer, BUFFER_SIZE, gain);

		//play sound
		writeToAudioInterface(buffer, BUFFER_SIZE);

		// Zero out the buffer.
		memset(&buffer[0], 0, BUFFER_SIZE);

	}while(!quit);

	//close audio interfaces and free data
	closeAudioInterface();
	shutdownAudioInterface();
	free(buffer);

	return EXIT_SUCCESS;
}

/**
 * Adjusts the volume of sound by multiplying voltage by 2^gain
 */
void* adjustGain(void* bufferIn, int bufferLength, int gain){

	//convert char buffer to 16bit buffer
	int16_t * buffer = bufferIn;

	//for the size of buffer/2 (because the int16 buffer will have half as many elements)
	for(int i = 0; i < bufferLength/2; i++) {

		//adjust 16 bit chunks according to gain
		if(gain >= 0) {
			buffer[i] = buffer[i] << gain;
		}else {
			buffer[i] = buffer[i] >> (-1 * gain);
		}
	}

	return buffer;
}

/**
 * Adds values of two buffers together
 */
void* combineSounds(void* bufferInOut, void* bufferIn, int bufferLength){

	//convert to 16bit buffers
	int16_t * buffer1 = bufferInOut;
	int16_t * buffer2 = bufferIn;

	//for the size of buffer/2 (because the int16 buffer will have half as many elements)
	for(int i = 0; i < bufferLength/2; i++) {

		buffer1[i] = buffer1[i] + buffer2[i];
	}

	return bufferInOut;
}



