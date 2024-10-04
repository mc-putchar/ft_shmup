#include <curses.h>
#include <iostream>

int main(int ac, char **av) {
  if (ac > 1)
    std::cout << "Usage: " << av[0] << std::endl;
  return (0);
}
