
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>  // for usleep
#include <chrono>
#include <cstdio>
#include <cstdlib>  // for system()
#include <fstream>
#include <iostream>
#include <thread>
#include "GameConfig.hpp"

void display_story(GameConfig& config, int rows, int cols) {
	(void)cols;
    std::string line;
    int i = 0;
    if ((int)config.intro.size() == 0) {
        mvprintw(0, 0, "...loading story...");
    }
    if ((int)config.intro.size() > rows) {
        mvprintw(i + 1, 15, "%s", "Press Any Keys To Continue");
        refresh();
    } else {
        for (const std::string& line : config.intro) {
            mvprintw(i + 1, 15, "%s", line.c_str());
            i++;
            refresh();
            usleep(50000);
        }
    }
}

void init_game() {
    (void)initscr();
    (void)cbreak();
    (void)noecho();
    start_color();                          // allow colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // color pair 1
    keypad(stdscr, TRUE);                   // enable function and cursor keys
    (void)nonl();  // do not add newline when pressing enter
    cbreak();      // do not buffer input
    noecho();      // do not echo input
    curs_set(0);   // do not show cursor
}

int main(int ac, char** av) {
    if (ac > 1)
        std::cout << "Usage: " << av[0] << std::endl;

    usleep(100000);
    GameConfig& config = GameConfig::getConf();
    init_game();
    int rows, cols;
    // int frameCount = 0;
    double fps = 60.0;
    auto startTime = std::chrono::high_resolution_clock::now();
    // auto lastTime = startTime;
    int c = 0;

    while (1) {
        if (!config.storyPlayed) {
            // werase(stdscr);
            getmaxyx(stdscr, rows, cols);
            display_story(config, rows, cols);
            if ((c = getch()) != 27) {
                config.storyPlayed = true;
                continue;
            }
            break;
        } else {
            werase(stdscr);
            char fpsStr[10];
            snprintf(fpsStr, sizeof(fpsStr), "fps %.2f", fps);
            mvwprintw(stdscr, 0, cols - 10, "%s", fpsStr);

            attron(COLOR_PAIR(1));
            mvwprintw(stdscr, 0, 0, "Pressed: %d", c);
            attroff(COLOR_PAIR(1));
            c = getch();
            if (c == 27)
                break;

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsedTime = endTime - startTime;
            std::chrono::duration<double> remainingTime =
                std::chrono::duration<double>(1.0 / 60.0) - elapsedTime;
            if (remainingTime > std::chrono::duration<double>(0.0)) {
                std::this_thread::sleep_for(remainingTime);
            } else {
                mvwprintw(stdscr, 0, cols - 10, "fps ...");
            }
        }
    }
    endwin();
    return (0);
}
