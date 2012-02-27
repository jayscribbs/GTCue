// Jack Client class definition

#include "JackClient.h"

// Constructor
JackClient::JackClient (const char * name,
	jack_options_t options,
	jack_status_t * status) : 
	playback(false),
	fileCued(false) {

	// Open the jack client
	if ( (jackClientHandle = jack_client_open(name, options, status)) == NULL) {
		// Throw an exception here eventually
	}
	
	// Register output ports with jackd
	leftPort = jack_port_register (jackClientHandle,
					"Stereo Out Left",
					JACK_DEFAULT_AUDIO_TYPE,
					JackPortIsOutput, 0);
	rightPort = jack_port_register (jackClientHandle,
					"Stereo Out Right",
					JACK_DEFAULT_AUDIO_TYPE,
					JackPortIsOutput, 0);
	if (leftPort == 0 || rightPort == 0) {
		// Exception
	}

	jack_set_process_callback (jackClientHandle, jackProcess, 0);
}

// Destructor
JackClient::~JackClient() {
	jack_client_close(jackClientHandle);
	if (fileCued) delete cueBufferPtr;
}

// Jack callback handlers
void JackClient::jackShutdown(void * arg) {
        // Exceptions
}

int JackClient::jackProcess(jack_nframes_t nframes, void * arg) {
	
	// Do not waste resources if no file is cued
	// or we are paused
	if (fileCued && playback) {
//		jack_default_audio_sample_t jackBuffer[sndfileinfo.channels];
		jack_default_audio_sample_t * outputLeft;
		jack_default_audio_sample_t * outputRight;
		
		// Find out what we can write to jackd
		outputLeft = static_cast<jack_default_audio_sample_t *> (
				jack_port_get_buffer (leftPort, nframes));
		outputRight = static_cast<jack_default_audio_sample_t *> (
				jack_port_get_buffer (rightPort, nframes));
		
		// Write data to jackd
		for (int i = 0; i < nframes; i++) {
			outputLeft[i] = cueBufferPtr[position];
			position++;
			outputRight[i] = cueBufferPtr[position];
			position++;
			if (position == sndfileinfo.frames) {
				this->setPause();
				position = 0;
			}
		}
	}

	return 0;
}

// File Handling
bool JackClient::cueFile(const char * path) {
	
	// Stop playback
	this->setPause();

	// Free any previous cues/memory
	if (fileCued) delete cueBufferPtr;

	// Open the new file to cue
	sndfileinfo.format = 0;
	if ((sndfile = sf_open(path, SFM_READ, &sndfileinfo)) == NULL) {
		// Exception
	}
	
	if (sndfileinfo.channels != 2) {} // Handle mono to stereo here?

	// Allocate HUGE memory for the file to be cued FIXME: Double?
	cueBufferPtr = new float[sndfileinfo.frames * sndfileinfo.channels];

	// Transfer the cued file to memory
	sf_readf_float (sndfile, cueBufferPtr, sndfileinfo.frames);

	// Close the file and release some memory
	sf_close(sndfile);

	position = 0;
	fileCued = true;
	
	return true;
}

// Set methods for playback
void JackClient::setPause() {
	playback = false;
}

void JackClient::setPlay() {
	playback = true;
}
