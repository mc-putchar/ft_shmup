NAME := ft_shmup
AUTHORS := astavrop, lbrusa, mcutura

SRCDIR := src
INCDIR := include
BINDIR := build

SRCS := main Entity Enemy

BINS := $(SRCS:%=$(BINDIR)/%.o)

CXX := c++
CXXFLAGS := -Wall -Wextra -std=c++11
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

clean:
	rm -fr $(BINDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

