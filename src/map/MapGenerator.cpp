#include "MapGenerator.h"

MapGenerator::MapGenerator(NoiseParam* par,std::pair<int, int> chunk_spaw):noisegenerator(par){
    liste_des_terrains=TerrainManager::get_all_natural_terrain();
}

MapGenerator::MapGenerator():noisegenerator(new NoiseParam){
    liste_des_terrains=TerrainManager::get_all_natural_terrain();
}

NoiseParam* MapGenerator::get_param(){
    return noisegenerator.get_param();
}

std::pair<Chunk*, std::pair<int, int>> MapGenerator::generate_spawn(int chunk_x, int chunk_y) const {
    Chunk* chunk = generate_chunk(chunk_x, chunk_y);

    std::srand(std::time(nullptr));
    for (int t = 0; t < max_try; ++t) {
        int rx = std::rand() % CHUNK_SIZE;
        int ry = std::rand() % CHUNK_SIZE;
        Case* c = chunk->at(rx, ry);

        if (c && c->is_constructible()) {
            Structure* hub = StructureManager::creer(StructureType::HUB);
            if (!hub) {
                print_error("Impossible de créer un hub.");
                delete chunk;
                return {nullptr, {-1, -1}};
            }

            c->set_structure(hub, true);
            int world_x = chunk_x * CHUNK_SIZE + rx;
            int world_y = chunk_y * CHUNK_SIZE + ry;

            print_primaire(hub->get_name(), " placé en (", world_x, ",", world_y, ")");

            return {chunk, {world_x, world_y}};
        }
    }

    print_error("Impossible de générer un hub dans le chunk (", chunk_x, ",", chunk_y, ")");
    delete chunk;
    return {nullptr, {-1, -1}};
}



Chunk* MapGenerator::generate_chunk(int chunk_x, int chunk_y) const {
    Chunk* chunk = new Chunk(chunk_x, chunk_y);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            int world_x = chunk_x * CHUNK_SIZE + x;
            int world_y = chunk_y * CHUNK_SIZE + y;
            float alt = noisegenerator.altitude(world_x, world_y);
            float hum = noisegenerator.humidity(world_x, world_y);

            alt = (alt + 1.0f) / 2.0f;
            hum = (hum + 1.0f) / 2.0f;

            Case* c = new Case;
            Terrain* terrain = TerrainManager::get_best_terrain(alt, hum);
            c->set_terrain(terrain);

            if (terrain->get_ressource_par_defaut() != RessourceType::AUCUNE)
                c->set_ressource(RessourceManager::creer(terrain->get_ressource_par_defaut()), true);

            chunk->set_case(x, y, c);
        }
    }

    return chunk;
}
