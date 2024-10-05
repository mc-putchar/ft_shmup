
#include "GameConfig.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void GameConfig::loadConfig()
{
	ifstream file("config.txt");
	if (!file.is_open())
	{
		cerr << "error opening config file" << endl;
		return;
	}

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		string key;
		string value;
		if (!(iss >> key >> value))
		{
			cerr << "error reading config file" << endl;
			return;
		}
	}
	file.close();
}