NAME := ft_shmup
AUTHORS := astavrop, lbrusa, mcutura

SRCDIR := src
INCDIR := include
BINDIR := build

SRCS := main

BINS := $(SRCS:%=$(BINDIR)/%.o)

CXX := c++
CXXFLAGS := -Wall -Wextra 
# CXXFLAGS := -Werror -std=c++11
CPPFLAGS := -I$(INCDIR)
LDFLAGS := -lncurses

.PHONY: all, clean, fclean, re

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

