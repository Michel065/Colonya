#ifndef _MAP_GENERATOR_H
#define _MAP_GENERATOR_H 

#include "../Commun/includes.h"
#include "../Terrain/Terrain.h"
#include "Chunk.h"
#include "NoiseGenerator.h"


class MapGenerator {    
private:
    NoiseGenerator noisegenerator;
    std::vector<Terrain*> liste_des_terrains;
    std::pair<int, int> chunk_spawn = {0, 0};

public:
    MapGenerator();
    MapGenerator(NoiseParam* par,std::pair<int, int> chunk_spaw = {0, 0});
    NoiseParam* get_param();
    Chunk* generate_chunk(int chunk_x, int chunk_y) const;



};

#endif