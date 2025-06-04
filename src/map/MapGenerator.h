#ifndef _MAP_GENERATOR_H
#define _MAP_GENERATOR_H 

#include "../includes.h"
#include "PerlinNoise.h"
#include "Biome.h"
#include "Chunk.h"

class NoiseGenerator {
private:
    PerlinNoise noise;
    int octaves; 
    float persistence;

public:
    NoiseGenerator(unsigned int s = 42, int o = 4, float p = 0.5f)
        : noise(s), octaves(o), persistence(p) {}

    float altitude(int x, int y, float scale = 0.005f) const {
        return noise.noise_octaved(x * scale, y * scale, octaves, persistence);
    }

    float humidity(int x, int y, float scale = 0.01f, int decalage = 1000) const {
        return noise.noise_octaved(x * scale + decalage, y * scale + decalage, octaves, persistence);
    }
};


class MapGenerator {    
private:
    NoiseGenerator noisegenerator;
    std::vector<Biome*> liste_des_biomes;
    Biome* select_biome(float altitude, float humidity)const;

public:
    MapGenerator(unsigned int s=42 ,int o=4 ,float p=0.5f);

    Chunk generate_chunk(int chunk_x, int chunk_y) const;
};

#endif