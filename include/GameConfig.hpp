









#pragma once


class GameConfig {
public:
	static GameConfig& getInstance() {
		static GameConfig instance;
		return instance;
	}
	GameConfig(const GameConfig&) = delete;
	GameConfig& operator=(const GameConfig&) = delete;

	int setRows(int rows);
	int setCols(int cols);
	int setFrameRate(int frameRate);
	int getFPS();
	void load_config(void);
	void write_config(void);


	private:
	GameConfig() : storyPlayed(false), rows(0), cols(0), frameRate(60.0) {}
	bool storyPlayed;
	int rows;
	int cols;
	int frameRate;

};