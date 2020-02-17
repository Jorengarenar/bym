CXX = g++

CXXFLAGS = -std=gnu++11 -g
LIBS = -l ncurses
INCLUDES = -I include

SRC = src
OBJ = obj

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(SOURCES:$(SRC)/%.cpp=$(OBJ)/%.o)

PROGNAME = bvim

$(PROGNAME): $(SRC)/main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(INCLUDES) -o $(PROGNAME) $(OBJECTS)

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $(INCLUDES) -c  $< -o $@

clean:
	rm $(PROGNAME) $(OBJECTS)
