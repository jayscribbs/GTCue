// Jack Client class definition

#include "JackClient.h"
#include <jack/midiport.h>

//////////////////////////////////////////////////////////////////
// Static Callback that delegates to a member function
int _JACK_CALLBACK_(jack_nframes_t frames, void * arg) {
	return static_cast<JackClient*>(arg)->jackProcess(frames);
}
//////////////////////////////////////////////////////////////////

// Constructor
JackClient::JackClient (const char * name,
	jack_options_t options,
	jack_status_t * status,
	Configuration * conf) : 
	playback(false),
	fileCued(false),
	learningMidi(false),
	config(conf) {

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
	midiPort = jack_port_register (jackClientHandle,
					"MIDI Input",
					JACK_DEFAULT_MIDI_TYPE,
					JackPortIsInput, 0);
	if (leftPort == 0 || rightPort == 0 || midiPort == 0) {
		// Exception
	}

	jack_set_process_callback (jackClientHandle, _JACK_CALLBACK_, this);

	jack_activate(jackClientHandle);

	cueFile(config->programMap[config->currentIndex][config->currentProgram].c_str());
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

int JackClient::jackProcess(jack_nframes_t nframes) {
	// Do not waste resources if no file is cued
	// or we are paused

	jack_default_audio_sample_t * outputLeft;
	jack_default_audio_sample_t * outputRight;

	if (!playback) {
		outputLeft = static_cast<jack_default_audio_sample_t *> (
				jack_port_get_buffer (leftPort, nframes));
		outputRight = static_cast<jack_default_audio_sample_t *> (
				jack_port_get_buffer (rightPort, nframes));

		for (int i = 0; i < nframes; i++) {
			outputLeft[i] = 0;
			outputRight[i] = 0;
		}
	}

	if (fileCued && playback) {
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
			if (position == sndfileinfo.frames * sndfileinfo.channels) {
				this->setPause();
				position = 0;
			}
		}
	}

	// Handle any MIDI events
	handleMidi(nframes);

	return 0;
}

// File Handling
bool JackClient::cueFile(const char * path) {
	// Stop playback
	this->setPause();

	// Free any previous cues/memory
	if (fileCued) {
		delete cueBufferPtr;
		fileCued = false;
	}

	// Open the new file to cue
	sndfileinfo.format = 0;
	if ((sndfile = sf_open(path, SFM_READ, &sndfileinfo)) == NULL) {
		// Exception
		return false;
	}
	
	if (sndfileinfo.channels != 2) {} // Handle mono to stereo here?

	// Allocate HUGE memory for the file to be cued FIXME: Double?
	// FIXME: Would it be better to be multithreaded and buffer from disk?
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

void JackClient::togglePlay() {
	if (playback) setPause();
	else setPlay();
}

void JackClient::handleMidi(jack_nframes_t nframes) {
	jack_nframes_t ctr, frameCount;
	void * buffer;

	if (!(buffer = jack_port_get_buffer (midiPort, nframes))) return;
	
	frameCount = jack_midi_get_event_count (buffer);

	for (ctr = 0; ctr < frameCount; ctr++) {
		jack_midi_event_t event;

		// Get the events and process individually
		if (!(jack_midi_event_get (&event, buffer, ctr))) {
			if (event.size == 0) break;

			int type = event.buffer[0] & 0xf0;

			// Control Change
			if (type == 0xb0) {
				// Toggle playback if this is a learned CC
				if (event.buffer[1] == config->learnedMidiControl[0] &&
					event.buffer[2] == config->learnedMidiControl[1])
					togglePlay();

				// Store the MSB for indexing if this is a CC#0 message
				if (event.buffer[1] == 0x00) bankIndex = event.buffer[2];

				// Learn a new CC
				if (learningMidi) {
					config->learnedMidiControl[0] = event.buffer[1];
					config->learnedMidiControl[1] = event.buffer[2];
					toggleLearningMidi();
					config->saveConfig();
				}
			}
			
			// Program Change
			if (type == 0xc0) {
				cueFile(config->programMap[bankIndex][event.buffer[1]].c_str());
				config->currentIndex = bankIndex;
				config->currentProgram = event.buffer[1];
				config->saveConfig();
			}
		}
	}
}

void JackClient::toggleLearningMidi() {
	if (learningMidi) learningMidi = false;
	else learningMidi = true;
}

bool JackClient::isLearning() {
	return learningMidi;
}
