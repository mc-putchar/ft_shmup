#include <curses.h>
#include <iostream>
#include <chrono>
#include <thread>



using namespace std;

int main(int ac, char **av) {
  if (ac > 1)
    std::cout << "Usage: " << av[0] << std::endl;
  (void) initscr();

//   resizeterm(125, 280);

  // allow colors
  start_color();

  init_pair(1, COLOR_CYAN, COLOR_BLACK); // color pair 1
  keypad(stdscr, TRUE); // enable function and cursor keys
  (void) nonl();  // do not add newline when pressing enter
  (void) cbreak(); // do not buffer input
  noecho(); // do not echo input


  while (1) {
	auto startTime = std::chrono::high_resolution_clock::now();
	int rows, cols;
    getmaxyx(stdscr, rows, cols);
	mvwprintw(stdscr, 0, cols - 10, "fps 60");
    int c = getch();
    // printw("Pressed: %d\n", c);
    attron(COLOR_PAIR(1));
	mvwprintw(stdscr, 0, 0, "Pressed: %d", c);
	attroff(COLOR_PAIR(1));

	if (c == 27)
	  break;

	auto endTime = std::chrono::high_resolution_clock::now();
	chrono::duration<double> elapsedTime = endTime - startTime;

	chrono::duration<double> remainingTime = chrono::duration<double>(1.0/60.0) - elapsedTime;
	
	if (remainingTime > chrono::duration<double>(0.0)) {
		this_thread::sleep_for(remainingTime);
	}
	else {
		mvwprintw(stdscr, 0, cols - 10, "fps ...");
	}

  }

  endwin();
  return (0);
}
