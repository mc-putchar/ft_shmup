#include <curses.h>
#include <iostream>

int main(int ac, char **av) {
  if (ac > 1)
    std::cout << "Usage: " << av[0] << std::endl;
  (void) initscr();
  keypad(stdscr, TRUE);
  (void) nonl();
  (void) cbreak();
  //(void) noecho();
  echo();
  while (1) {
    int c = getch();
    printw("Pressed: %d\n", c);
  }
  endwin();
  return (0);
}
