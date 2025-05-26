# Variables
CXX = g++
CXXFLAGS = -c -Wall -std=c++17
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Cibles

all: main

# Compilation des fichiers objets
main.o: src/main.cpp src/core/Map.hpp src/core/Tile.hpp src/core/TerrainType.hpp src/core/MapGenerator.hpp
	$(CXX) $(CXXFLAGS) -o build/main.o src/main.cpp
	
Map.o: src/core/Map.cpp src/core/Map.hpp src/core/Tile.hpp src/core/TerrainType.hpp
	$(CXX) $(CXXFLAGS) -o build/Map.o src/core/Map.cpp
	
Tile.o: src/core/Tile.cpp src/core/Tile.hpp
	$(CXX) $(CXXFLAGS) -o build/Tile.o src/core/Tile.cpp
	
TerrainType.o: src/core/TerrainType.cpp src/core/TerrainType.hpp
	$(CXX) $(CXXFLAGS) -o build/TerrainType.o src/core/TerrainType.cpp
	
MapGenerator.o: src/core/MapGenerator.cpp src/core/MapGenerator.hpp src/core/Map.hpp
	$(CXX) $(CXXFLAGS) -o build/MapGenerator.o src/core/MapGenerator.cpp

# Compilation de l'exécutable final
main: build/main.o build/Map.o build/Tile.o build/TerrainType.o build/MapGenerator.o
	$(CXX) -o build/simulation build/main.o build/Map.o build/Tile.o build/TerrainType.o build/MapGenerator.o $(SFMLFLAGS)

# Nettoyage des fichiers objets
clean:
	rm -f build/*.o build/simulation
