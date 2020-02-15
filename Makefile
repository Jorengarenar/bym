CC = g++

CXXFLAGS = -std=gnu++11 -g -Wall
LIBS = -l ncurses
INCLUDES = -I include

SRC = src

PROGNAME = bvim

$(PROGNAME): $(SRC)/main.cpp $(SRC)/buffer.cpp $(SRC)/window.cpp
	$(CC) $(CXXFLAGS) $(LIBS) $(INCLUDES) -o $(PROGNAME) $(SRC)/main.cpp $(SRC)/buffer.cpp $(SRC)/window.cpp
