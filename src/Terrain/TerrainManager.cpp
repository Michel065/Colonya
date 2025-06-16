#include "TerrainManager.h"

void TerrainManager::register_terrain(const std::string& name, Terrain* terrain) {
    terrain->texture=textures_file+terrain->texture;
    terrain_map[name] = terrain;
    if (name == "defaut") defaut = terrain;
}

Terrain* TerrainManager::get(const std::string& name) {
    auto it = terrain_map.find(name);
    if (it != terrain_map.end()) return it->second;
    return get_defaut_terrain();
}

Terrain* TerrainManager::get_defaut_terrain() {
    return defaut;
}

std::vector<std::string> TerrainManager::get_liste_terrain() {
    std::vector<std::string> names;
    for (const auto& [name, terrain] : terrain_map) {
        names.push_back(name);
    }
    return names;
}

void TerrainManager::print_liste_terrain(){
    std::string salut="listes des terrains dispo:{";
    for (const auto& [name, terrain] : terrain_map) {
        salut+=name+", ";
    }
    salut+="}";
    print_primaire(salut);
}


std::vector<Terrain*> TerrainManager::get_all_terrain(){
    std::vector<Terrain*> ter;
    for (auto& [name, terrain] : terrain_map) {
        ter.push_back(terrain);
    }
    return ter;
}

std::vector<Terrain*> TerrainManager::get_all_natural_terrain(){
    std::vector<Terrain*> ter;
    for (auto& [name, terrain] : terrain_map) {
        if(terrain->is_natural){
            ter.push_back(terrain);
        }
    }
    return ter;
}

void TerrainManager::clear() {
    for (auto& pair : terrain_map) {
        delete pair.second; // libère chaque Terrain*
    }
    terrain_map.clear();
    defaut = nullptr;
}


Terrain* TerrainManager::get_best_terrain(float altitude, float humidity) {
    Terrain* best = defaut;
    float best_score = -1.0f;

    for (auto& [name, terrain] : terrain_map) {
        float score = terrain->match_score(altitude, humidity);
        if (score > best_score) {
            best = terrain;
            best_score = score;
        }
    }

    return best;
}
