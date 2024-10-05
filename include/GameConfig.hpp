









#pragma once

#include <string>
#include <vector>
class GameConfig {
public:
	static GameConfig& getConf() {
		static GameConfig instance;
		// init_instance();
		instance.storyPlayed = false;
		instance.loadConfig();
		instance.loadStory();
		return instance;
	}
	GameConfig(const GameConfig&) = delete;
	GameConfig& operator=(const GameConfig&) = delete;

	int setRows(int rows);
	int setCols(int cols);
	int setFrameRate(int frameRate);
	int getFPS();
	void loadConfig(void);
	void loadStory(void);
	
	void write_config(void);

	bool storyPlayed;
	std::vector<std::string> intro;

	private:
	GameConfig() : storyPlayed(false), rows(0), cols(0), frameRate(60.0) {}
	int rows;
	int cols;
	int frameRate;


};