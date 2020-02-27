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
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(PROGNAME) $(OBJECTS) $(LIBS)

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c  $< -o $@ $(LIBS)

clean:
	rm $(PROGNAME) $(OBJECTS)
