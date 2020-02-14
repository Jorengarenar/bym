CC = g++

CFLAGS = -std=gnu++11 -g
LIBS = -l ncurses

SRC = src

MAIN = bvim

$(MAIN): $(SRC)/$(MAIN).cpp $(SRC)/buffer.cpp
	$(CC) $(CFLAGS) $(LIBS) -o $(MAIN) $(SRC)/$(MAIN).cpp $(SRC)/buffer.cpp
