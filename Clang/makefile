# amateur attempt at a makefile

SHELL = /bin/sh
CXX = clang++
FLAGS = -std=c++11 -stdlib=libc++ -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit
CXXFLAGS = -Wall

TARGET = game
SOURCES = $(shell echo ../Source/*.cpp)
HEADERS = $(shell echo ../Source/*.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(FLAGS) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
