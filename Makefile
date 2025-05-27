# Variables
CXX = g++
CXXFLAGS = -c -Wall -std=c++17
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Cibles

all: main

# Compilation des fichiers objets
build/main.o: src/main.cpp src/main.h
	$(CXX) $(CXXFLAGS) -o build/main.o src/main.cpp

# Compilation de l'exécutable final
main: build/main.o 
	$(CXX) -o main build/main.o $(SFMLFLAGS) 	

# Nettoyage des fichiers objets
clean:
	rm -f build/*.o build/simulation
