#ifndef _MAP_GENERATOR_H
#define _MAP_GENERATOR_H 

#include "../includes.h"
#include "PerlinNoise.h"
#include "Biome.h"
#include "Chunk.h"

class NoiseGenerator{
private:
    PerlinNoise* noise;
    int octaves; 
    float persistence;
    unsigned int seed;
public:
    NoiseGenerator(unsigned int s=42 ,int o=4 ,float p=0.5f){
        octaves = s; 
        persistence = o;
        seed = p; 
        noise = PerlinNoise(s);
    }

    float altitude(int x, int y,float scale=0.005f){
        return noise.noise_octaved(x * scale, y *scale,octaves,persistence);
    }

    float humidite(int x, int y,float scale=0.01f, int decalage=1000){
        return noise.noise_octaved(x * scale + decalage, y * scale + decalage,octaves,persistence);
    }

};


class MapGenerator {    
private:
    unsigned int seed;
    float altitude_scale = 0.01f;
    float humidity_scale = 0.01f;

    std::vector<Biome> biomes;
    std::mt19937 rng;

    float noise2D(float x, float y, float scale) const;

    const Biome& select_biome(float altitude, float humidity) const;

public:
    MapGenerator(unsigned int seed_init = 42);

    void register_biome(const Biome& biome);

    Chunk generate_chunk(int chunk_x, int chunk_y) const;
};

#endif