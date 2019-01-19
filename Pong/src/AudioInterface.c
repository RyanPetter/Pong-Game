/*
 * AudioInterface.c
 *
 *  Created on: Sep 17, 2016
 *      Author: cs3841
 */

#include "AudioInterface.h"

	/**
	 * This is the name of the device.  It is a pointer that will be allocated upon instantiation.
	 */
	static char* deviceName;
	/**
	 * This integer represents the sampling rate for the system.
	 */
	static unsigned int samplingRate;
	/**
	 * This is the number of channels.  2 will be normal for stereo.
	 */
	static int channels;
	/**
	 * This is the direction of the class.  It is either  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
	 */
	static snd_pcm_stream_t direction;
	/**
	 * This is a pointer to a device handle.
	 */
	static snd_pcm_t *handle; // This is a pointer to the soundcard device.

	/**
	 * This is a variable which holds the number of frames.
	 */
	static snd_pcm_uframes_t frames;

	/**
	 * This is a pointer to the hardware set of parameters.
	 */
	static snd_pcm_hw_params_t *params;

	/**
	 * This is the constructor for the class.  It will instantiate a new instance of the class.
	 * @param deviceName This is the name of the device.  A typical name might be "plughw:1"
	 * @param samplingRate This is the sampling rate for the audio sampls.  Typical rates are 11000, 22050, and 44100.
	 * @param channels This is the number of channels.  Typically this is 1 or 2.
	 * @param direction This is the direction.  It is either  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
	 */
	void initializeAudioInterface(char* pdeviceName, unsigned int psamplingRate, int pchannels, snd_pcm_stream_t pdirection)
	{
		int deviceLength = strlen(pdeviceName);
		deviceName = (char*) malloc(deviceLength + 1);
		// Initialize the buffer with the device name.
		strcpy(deviceName, pdeviceName);
		samplingRate = psamplingRate;
		channels = pchannels;
 		direction = pdirection;
		handle = NULL;
		params = NULL;
		frames = DEFAULT_FRAME_SIZE;
	}
	/**
	 * This is the destructor.  It will clear all allocated space.
	 */
	void shutdownAudioInterface()
	{
		free(deviceName);
	}

	/**
	 * This method will open the given device so that it can be accessed.
	 */
	void openAudioInterface()
	{

		int rc; // This variable is to be used to store the return code from various calls.
			int dir;

			/* Open PCM device. */
			snd_pcm_open(&handle, deviceName, direction, 0);

			/* Allocate a hardware parameters object. */
			snd_pcm_hw_params_alloca(&params);

			/* Fill it in with default values. */
			snd_pcm_hw_params_any(handle, params);

			/* Set the desired hardware parameters. */

			/* Interleaved mode */
			snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

			/* Signed 16-bit little-endian format */
			snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

			/* Two channels (stereo) */
			snd_pcm_hw_params_set_channels(handle, params, channels);

			// Set the sampling rate appropriately.
			snd_pcm_hw_params_set_rate_near(handle, params, &(samplingRate), &dir);

			/* Set period size to the given number of frames. */
			snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

			/* Write the parameters to the driver */
			rc = snd_pcm_hw_params(handle, params);

			// Prepare the soundcard for data.
			snd_pcm_prepare(handle);

			if (rc < 0) {
				fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
				exit(1);
			}

	}

	/**
	 * This method will close the given device so it is no longer accessible.
	 */
	void closeAudioInterface()
	{
		snd_pcm_drain(handle);
		snd_pcm_close(handle);
	}

	/**
	 * This method will write data to the soundcard for playback.
	 * @param buffer This is the buffer that is to be written.  It must be of size getRequiredBufferisze().
	 * @param bufferLength This is the number of valid entries in the buffer.
	 */
	void writeToAudioInterface(char* buffer, int bufferLength)
	{
		int rc; // This variable is to be used to store the return code from various calls.
		int frames = bufferLength / (2 * channels);

		if (bufferLength > 0) {
			// Write the data to the soundcard.
			rc = snd_pcm_writei(handle, buffer, frames);

			if (rc == -EPIPE) {
				/* EPIPE means underrun */
				//fprintf(stderr, "underrun occurred\n");
				snd_pcm_prepare(handle);
			} else if (rc < 0) {
				fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
			} else if (rc != (int) frames) {
				fprintf(stderr, "short write, write %d frames\n", rc);
			}
		}
	}

	/**
	 * This method will determine, based upon the size of each frame and the number of frames between interrupts the required size of the buffer that is to store the data.
	 * @return
	 */
	int getRequiredAudioBufferSize()
	{
		int dir;
		int size;
		snd_pcm_hw_params_get_period_size(params, &frames, &dir);
		size = frames * channels * 2; /* 2 bytes/sample */
		return size;
	}
