#include "MapGenerator.h"

Biome* MapGenerator::select_biome(float altitude, float humidity)const{
    Biome* best = nullptr;
    float best_score = -1.0f;
    for (Biome* biome : liste_des_biomes) {
        float score = biome->match_score(altitude, humidity);
        if (score > best_score) {
            best_score = score;
            best = biome;
        }
    }
    return (best) ? best : BiomeManager::get_defaut_biome();
}



MapGenerator::MapGenerator(unsigned int s ,int o ,float p):noisegenerator(s,o,p){
    liste_des_biomes=BiomeManager::get_all_natural_biome();
}

Chunk MapGenerator::generate_chunk(int chunk_x, int chunk_y) const{
    Chunk chunk(chunk_x, chunk_y);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            int world_x = chunk_x * CHUNK_SIZE + x;
            int world_y = chunk_y * CHUNK_SIZE + y;

            float alt = noisegenerator.altitude(world_x, world_y);
            float hum = noisegenerator.humidity(world_x, world_y);

            Case* c =new Case;
            c->set_biome(select_biome(alt, hum));
            chunk.set_case(x, y, c);
        }
    }
    return chunk;
}