// Configuration Object
// This will be saved in a binary file
// Makes it a simple load/save with no parsing required

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include<string>

using namespace std;

class Configuration {

	public:
		Configuration(string file);
		~Configuration();

		bool loadConfig();
		bool saveConfig();

	protected:
		string path;
		string programMap[4][100];
};

#endif
