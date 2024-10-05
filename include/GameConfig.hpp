









#pragma once


class GameConfig {

	public:
	
	int setRows(int rows);
	int setCols(int cols);
	int setFrameRate(int frameRate);
	int getFPS();

	load_config();
	write_config();

	bool storyPlayed;

	private:
	int rows;
	int cols;
	int frameRate;

}