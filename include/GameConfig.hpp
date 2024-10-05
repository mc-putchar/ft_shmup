









#pragma once


class GameConfig {
public:
	static GameConfig& getConf() {
		static GameConfig instance;
		// init_instance();
		instance.storyPlayed = false;
		instance.loadConfig();
		return instance;
	}
	GameConfig(const GameConfig&) = delete;
	GameConfig& operator=(const GameConfig&) = delete;

	int setRows(int rows);
	int setCols(int cols);
	int setFrameRate(int frameRate);
	int getFPS();
	void loadConfig(void);
	void write_config(void);

	bool storyPlayed;

	private:
	GameConfig() : storyPlayed(false), rows(0), cols(0), frameRate(60.0) {}
	int rows;
	int cols;
	int frameRate;

};