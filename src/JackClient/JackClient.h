// Jack Client object
// Reads MIDI and writes audio data to jackd

#ifndef JACKCLIENT_H
#define JACKCLIENT_H

#include <jack/jack.h>
#include "../Configuration.h"
#include <sndfile.h>

class JackClient {

	public:
		// Constructor
		JackClient(const char * name,
			jack_options_t options,
			jack_status_t * status,
			Configuration * conf);

		// Destructor
		~JackClient();

		// Jack callback handlers
		void jackShutdown(void * arg);
		int jackProcess(jack_nframes_t nframes);

		//File handling/cueing
		bool cueFile(const char * path);

		// Get/Set methods for playback
		int getPosition();
		void setPosition(int value);
		int getLength();
		void setPause();
		void setPlay();
		void setVolume(double value);
		void togglePlay();

		// MIDI processing
		void handleMidi(jack_nframes_t nframes);
		void toggleLearningMidi();
		bool isLearning();

	protected:
		// Jack client details
		jack_client_t * jackClientHandle;

		// Audio Ports
		jack_port_t * leftPort;
		jack_port_t * rightPort;

		// MIDI Port and Indexes
		jack_port_t * midiPort;
		char bankIndex;

		// Buffer control and libsndfile data
		float * cueBufferPtr;
		SNDFILE * sndfile;
		SF_INFO sndfileinfo;
		int position;
		double volume;

		//Configuration Object Pointer
		Configuration * config;
		
		// Various flags
		bool playback;
		bool fileCued;
		bool learningMidi;
};

#endif
