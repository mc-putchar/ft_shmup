#include <curses.h>
#include <iostream>

int main(int ac, char **av) {
  if (ac > 1)
    std::cout << "Usage: " << av[0] << std::endl;
  (void) initscr();
  // Set the terminal size to 25 rows and 80 columns
  resize_term(25, 80);


  keypad(stdscr, TRUE);
  (void) nonl();
  (void) cbreak();
  //(void) noecho();
  echo();
  while (1) {
    int c = getch();
    // printw("Pressed: %d\n", c);
	mvwprintw(stdscr, 0, 0, "Pressed: %d", c);

	if (c == 27)
	  break;
  }
  endwin();
  return (0);
}
