# Variables
CXX = g++
CXXFLAGS = -c -Wall -std=c++17
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Cibles

all: main

build/main.o: src/main.cpp src/main.h src/Synchronisation/TimeManager.h
	$(CXX) $(CXXFLAGS) -o build/main.o src/main.cpp

main: build/main.o
	$(CXX) -o main build/main.o $(SFMLFLAGS) 	

clean:
	rm -f build/*.o main
