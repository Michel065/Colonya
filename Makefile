# Variables
CXX = g++
CXXFLAGS = -c -Wall -std=c++17 -Ilib
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Cibles

all: main

build/main.o: src/main.cpp src/main.h src/Synchronisation/TimeManager.h src/includes.h src/map/MapManager.h src/map/Case.h src/map/BiomeManager.h 
	$(CXX) $(CXXFLAGS) -o build/main.o src/main.cpp

build/mapmanager.o: src/map/MapManager.cpp src/map/MapManager.h src/map/Map.h src/Synchronisation/TimeManager.h src/map/MapContexte.h src/includes.h
	$(CXX) $(CXXFLAGS) -o build/mapmanager.o src/map/MapManager.cpp

build/map.o: src/map/Map.cpp src/map/Map.h src/includes.h src/map/Chunk.h
	$(CXX) $(CXXFLAGS) -o build/map.o src/map/Map.cpp


build/chunk.o: src/map/Chunk.cpp src/map/Chunk.h src/map/Case.h src/includes.h
	$(CXX) $(CXXFLAGS) -o build/chunk.o src/map/Chunk.cpp
	
build/BiomeManager.o: src/map/BiomeManager.cpp src/map/BiomeManager.h src/map/Biome.h src/map/Case.h
	$(CXX) $(CXXFLAGS) -o build/BiomeManager.o src/map/BiomeManager.cpp

build/includes.o: src/includes.cpp src/includes.h 
	$(CXX) $(CXXFLAGS) -o build/includes.o src/includes.cpp

main: build/main.o build/mapmanager.o  build/includes.o build/chunk.o build/BiomeManager.o  build/map.o 
	$(CXX) -o main build/main.o build/mapmanager.o build/includes.o build/BiomeManager.o build/map.o build/chunk.o  $(SFMLFLAGS) 	

clean:
	rm -f build/*.o main

c:
	rm -f build/*.o main

run:
	./main

r:
	./main

tout:
	make clean
	make
	make run