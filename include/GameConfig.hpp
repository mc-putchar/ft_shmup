









#pragma once


class GameConfig {

	public:
	
	int setRows(int rows);
	int setCols(int cols);
	int setFrameRate(int frameRate);
	int getFPS();

	void load_config(void);
	void write_config(void);


	private:
	bool storyPlayed;
	int rows;
	int cols;
	int frameRate;

}