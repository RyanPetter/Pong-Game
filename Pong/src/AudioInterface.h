/*
 * AudioInterface.h
 *
 *  Created on: Apr 18, 2015
 *      Author: Walter Schilling
 *      This class provides an interface to the audio system that we will be uisng.
 *      It allows the user to open a device for both capture and playback, and it sets the frame isze accordingly.
 */

#ifndef AUDIOINTERFACE_H_
#define AUDIOINTERFACE_H_

#include <alsa/asoundlib.h>

#define DEFAULT_FRAME_SIZE 512


/**
 * This is the constructor for the class.  It will instantiate a new instance of the class.
 * @param deviceName This is the name of the device.  A typical name might be "plughw:1"
 * @param samplingRate This is the sampling rate for the audio sampls.  Typical rates are 11000, 22050, and 44100.
 * @param channels This is the number of channels.  Typically this is 1 or 2.
 * @param direction This is the direction.  It is either  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
 */
void initializeAudioInterface(char* pdeviceName, unsigned int psamplingRate,
		int pchannels, snd_pcm_stream_t pdirection);

/**
 * This is the destructor.  It will clear all allocated space.
 */
void shutdownAudioInterface();

/**
 * This method will open the given device so that it can be accessed.
 */
void openAudioInterface();


/**
 * This method will close the given device so it is no longer accessible.
 */
void closeAudioInterface();

/**
 * This method will write data to the soundcard for playback.
 * @param buffer This is the buffer that is to be written.  It must be of size getRequiredBufferisze().
 * @param bufferLength This is the number of valid entries in the buffer.
 */
void writeToAudioInterface(char* buffer, int bufferLength);

/**
 * This method will determine, based upon the size of each frame and the number of frames between interrupts the required size of the buffer that is to store the data.
 * @return
 */
int getRequiredAudioBufferSize();

#endif /* AUDIOINTERFACE_H_ */
