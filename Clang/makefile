# amateur attempt at a makefile

SHELL = /bin/sh
CXX = clang++
FLAGS = -std=c++11 -stdlib=libc++ -lGLEW -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit
CXXFLAGS = -Wall

TARGET = game
SOURCES = $(shell echo ../Source/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(FLAGS) $(CXXFLAGS) $(INC_DIRS) -o $(TARGET) $(SOURCES)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
