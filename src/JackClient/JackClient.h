// Jack Client object
// Reads MIDI and writes audio data to jackd

#ifndef JACKCLIENT_H
#define JACKCLIENT_H

#include<jack/jack.h>
#include<sndfile.h>

using namespace std;

class JackClient {

	public:
		// Constructor
		JackClient(const char * name,
			jack_options_t options,
			jack_status_t * status);

		// Destructor
		~JackClient();

		// Jack callback handlers
		void jackShutdown(void * arg);
		int jackProcess(jack_nframes_t nframes, void * arg);

		//File handling/cueing
		bool cueFile(const char * path);

		// Set methods for playback
		void setPause();
		void setPlay();

	protected:
		// Jack client details
		jack_client_t * jackClientHandle;
		jack_port_t * leftPort;
		jack_port_t * rightPort;

		// Buffer control and libsndfile data
		float * cueBufferPtr;
		SNDFILE * sndfile;
		SF_INFO sndfileinfo;
		int position;
		
		// Various flags
		bool playback;
		bool fileCued;
};

#endif
