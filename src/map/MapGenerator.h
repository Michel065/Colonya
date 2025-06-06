#ifndef _MAP_GENERATOR_H
#define _MAP_GENERATOR_H 

#include "../includes.h"
#include "Biome.h"
#include "Chunk.h"
#include "NoiseGenerator.h"


class MapGenerator {    
private:
    NoiseGenerator noisegenerator;
    std::vector<Biome*> liste_des_biomes;
    std::pair<int, int> chunk_spawn = {0, 0};

public:
    MapGenerator(NoiseParam* par,std::pair<int, int> chunk_spaw = {0, 0});
    NoiseParam* get_param();
    Chunk* generate_chunk(int chunk_x, int chunk_y) const;



};

#endif