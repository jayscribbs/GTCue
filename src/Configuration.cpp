// Configuration object class definition

#include "Configuration.h"
#include <fstream>
#include <string>
#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>

using namespace std;

Configuration::Configuration (string file) {
	path = Glib::build_filename(Glib::get_user_config_dir(), file);
	loadConfig();
}

Configuration::~Configuration() {}

bool Configuration::loadConfig() {
	ifstream f (path.c_str());
	if (!f.is_open()) {
		//Initialize the file with empties to prevent load bugs
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j<100; j++) {
				programMap[i][j] = "EMPTY";
			}
		}
		saveConfig();
		return false;
	}

	f>>currentIndex;
	f>>currentProgram;
	f>>learnedMidiControl[0];
	f>>learnedMidiControl[1];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 100; j++) {
			f>>programMap[i][j];
		}
	}
	f.close();

	return true;
}

bool Configuration::saveConfig() {
	ofstream f (path.c_str());
	if (!f.is_open()) return false;

	f<<currentIndex<<endl;
	f<<currentProgram<<endl;
	f<<learnedMidiControl[0]<<endl;
	f<<learnedMidiControl[1]<<endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 100; j++) {
			f<<programMap[i][j]<<endl;
		}
	}

	f.close();

	return true;
}
