
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <ncurses.h>
#include <cstdlib>	// for system()
#include <unistd.h> // for usleep
#include <fcntl.h>
#include "GameConfig.hpp"

using namespace std;

void display_story(int rows, int cols)
{

	ifstream file("logo.txt");
	if (!file.is_open()) {
		mvprintw(0,0,"error opening start story file");
	}

	string line;
	int i = 0;
	while (getline(file, line) && i < rows) {

		mvprintw(i + 3, 15, "%s", line.c_str());
		i++;
		refresh();
		usleep(50000);
	}
	file.close();
}

void init_game()
{
	(void)initscr();
	usleep(50000);

	start_color();						   // allow colors
	init_pair(1, COLOR_CYAN, COLOR_BLACK); // color pair 1
	keypad(stdscr, TRUE);				   // enable function and cursor keys
	(void)nonl();						   // do not add newline when pressing enter
	(void)cbreak();						   // do not buffer input
	noecho();							   // do not echo input
	curs_set(0);						   // do not show cursor
}

int main(int ac, char **av)
{
	if (ac > 1)
		std::cout << "Usage: " << av[0] << std::endl;
	GameConfig& config = GameConfig::getInstance();
	init_game();
	int rows, cols;
	// int frameCount = 0;
	double fps = 60.0;
	auto startTime = chrono::high_resolution_clock::now();
	// auto lastTime = startTime;

	while (1)
	{
		werase(stdscr);
		getmaxyx(stdscr, rows, cols);
		// getmaxyx(stdscr, rows, cols);
		// WINDOW *win = newwin(rows, cols, 0, 0);
		display_story(rows, cols);
		char fpsStr[10];
		snprintf(fpsStr, sizeof(fpsStr), "fps %.2f", fps);
		mvwprintw(stdscr, 0, cols - 10, "%s", fpsStr);

		int c = getch();

		attron(COLOR_PAIR(1));
		mvwprintw(stdscr, 0, 0, "Pressed: %d", c);
		attroff(COLOR_PAIR(1));

		if (c == 27)
			break;

		auto endTime = std::chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedTime = endTime - startTime;
		chrono::duration<double> remainingTime = chrono::duration<double>(1.0 / 60.0) - elapsedTime;
		if (remainingTime > chrono::duration<double>(0.0))
		{
			this_thread::sleep_for(remainingTime);
		}
		else
		{
			mvwprintw(stdscr, 0, cols - 10, "fps ...");
		}
	}

	endwin();
	return (0);
}
