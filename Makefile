# Variables
CXX = g++
CXXFLAGS = -c -Wall -std=c++17 -Ilib
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Cibles

all: main

build/main.o: src/main.cpp src/main.h src/Synchronisation/TimeManager.h src/includes.h src/map/MapManager.h
	$(CXX) $(CXXFLAGS) -o build/main.o src/main.cpp

build/mapmanager.o: src/map/MapManager.cpp src/map/MapManager.h src/map/Map.h src/Synchronisation/TimeManager.h src/map/MapContexte.h src/includes.h
	$(CXX) $(CXXFLAGS) -o build/mapmanager.o src/map/MapManager.cpp

build/mapcontextejson.o: src/map/serialization/MapContexteJson.cpp src/map/MapContexte.h 
	$(CXX) $(CXXFLAGS) -o build/mapcontextejson.o src/map/serialization/MapContexteJson.cpp


main: build/main.o build/mapmanager.o build/mapcontextejson.o
	$(CXX) -o main build/main.o build/mapmanager.o build/mapcontextejson.o $(SFMLFLAGS) 	

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