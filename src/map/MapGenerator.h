#ifndef _MAP_GENERATOR_H
#define _MAP_GENERATOR_H 

#include "../Commun/includes.h"
#include "../Terrain/Terrain.h"

#include "Chunk.h"
#include "NoiseGenerator.h"
#include "Map.h"

#include "../Structure/Structure.h"
#include "../Structure/all_Structure.h"

class MapGenerator {    
private:
    NoiseGenerator noisegenerator;
    std::vector<Terrain*> liste_des_terrains;
    int max_try = 100;//pour le spawn du hub ou structure

public:
    MapGenerator();
    MapGenerator(NoiseParam* par,std::pair<int, int> chunk_spaw = {0, 0});
    NoiseParam* get_param();
    std::pair<Chunk*, std::pair<int, int>> generate_spawn(int chunk_x, int chunk_y) const;
    Chunk* generate_chunk(int chunk_x, int chunk_y) const;
};

#endif