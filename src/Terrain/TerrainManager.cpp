#include "TerrainManager.h"

void TerrainManager::add_terrain(const std::string& name, const std::string& texture,bool walkable,
                             bool constructible,float alt_target, float hum_target,bool is_natural,
                             std::function<void(Case&)> evolution_logic,float score_boost){
    terrain_map[name] = Terrain{name,textures_file + texture,walkable,constructible,alt_target,
                            hum_target,score_boost,is_natural,evolution_logic};
}

void TerrainManager::load_terrains() {
    add_terrain("eau", "eau.png", false, false,
              0.15f, 0.85f, true, nullptr, 1.2f);   // zone basse et très humide

    add_terrain("sable", "sable.png", true, true,
              0.25f, 0.65f, true, nullptr, 1.01f);   // autour de l’eau, plutôt sec

    add_terrain("plaine", "terre.png", true, true,
              0.4f, 0.55f, true, nullptr, 1.0f);    // moyennement bas et humide

    add_terrain("herbe", "herbe.png", true, true,
              0.5f, 0.65f, true, nullptr, 1.0f);    // neutre

    add_terrain("roche", "roche.png", true, true,
              0.65f, 0.3f, true, nullptr, 0.9f);    // plus haut et sec

    add_terrain("montagne", "montagne.png", false, false,
              0.9f, 0.2f, true, nullptr, 1.05f);     // très haut et sec
}





Terrain* TerrainManager::get(const std::string& name) {
    auto it = terrain_map.find(name);
    return (it != terrain_map.end() ? &it->second : &defaut);
}

Terrain* TerrainManager::get_defaut_terrain() {
    return &defaut;
}

std::vector<std::string> TerrainManager::get_liste_terrain() {
    std::vector<std::string> names;
    for (const auto& [name, terrain] : terrain_map) {
        names.push_back(name);
    }
    return names;
}

std::vector<Terrain*> TerrainManager::get_all_terrain(){
    std::vector<Terrain*> ter;
    for (auto& [name, terrain] : terrain_map) {
        ter.push_back(&terrain);
    }
    return ter;
}

std::vector<Terrain*> TerrainManager::get_all_natural_terrain(){
    std::vector<Terrain*> ter;
    for (auto& [name, terrain] : terrain_map) {
        if(terrain.is_natural){
            ter.push_back(&terrain);
        }
    }
    return ter;
}

void TerrainManager::clear() {
    terrain_map.clear();
}

Terrain* TerrainManager::get_best_terrain(float altitude, float humidity) {
    Terrain* best = &defaut;
    float best_score = -1.0f;

    for (auto& [name, terrain] : terrain_map) {
        float score = terrain.match_score(altitude, humidity);
        if (score > best_score) {
            best = &terrain;
            best_score = score;
        }
    }

    return best;
}
