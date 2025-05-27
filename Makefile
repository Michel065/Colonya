# Variables
CXX = g++
CXXFLAGS = -c -Wall -std=c++17
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Cibles

all: main

build/main.o: src/main.cpp src/main.h
	$(CXX) $(CXXFLAGS) -o build/main.o src/main.cpp

build/TimeManager.o: src/Synchronisation/TimeManager.cpp src/Synchronisation/TimeManager.h
	$(CXX) $(CXXFLAGS) -o build/TimeManager.o src/Synchronisation/TimeManager.cpp

main: build/main.o build/TimeManager.o
	$(CXX) -o main build/main.o build/TimeManager.o $(SFMLFLAGS) 	

clean:
	rm -f build/*.o main
