NAME := ft_shmup
AUTHORS := astavrop, lbrusa, mcutura

SRCDIR := src
INCDIR := include
BINDIR := build

SRCS := main draw Entity Enemy Weapon GameConfig Player Game

BINS := $(SRCS:%=$(BINDIR)/%.o)

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -pedantic -ggdb3 -Og
CPPFLAGS := -I$(INCDIR)
LDFLAGS := -lncurses

.PHONY: all, clean, fclean, re, run

all: $(NAME)

$(NAME): $(BINS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(CPPFLAGS) $(BINS) $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

run: all
	@PATH=".$${PATH:+:$${PATH}}" && alacritty --config-file alacritty_config.toml --command $(NAME)

clean:
	rm -fr $(BINDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
