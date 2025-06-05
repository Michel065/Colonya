#include "BiomeManager.h"

void BiomeManager::add_biome(const std::string& name, const std::string& texture,bool walkable,
                             bool constructible,float alt_target, float hum_target,bool is_natural,
                             std::function<void(Case&)> evolution_logic,float score_boost){
    biome_map[name] = Biome{name,textures_file + texture,walkable,constructible,alt_target,
                            hum_target,score_boost,is_natural,evolution_logic};
}

void BiomeManager::load_biomes() {
    add_biome("eau", "eau.png", false, false,
              0.15f, 0.85f, true, nullptr, 1.2f);   // zone basse et très humide

    add_biome("sable", "sable.png", true, true,
              0.25f, 0.65f, true, nullptr, 1.01f);   // autour de l’eau, plutôt sec

    add_biome("plaine", "terre.png", true, true,
              0.4f, 0.55f, true, nullptr, 1.0f);    // moyennement bas et humide

    add_biome("herbe", "herbe.png", true, true,
              0.5f, 0.65f, true, nullptr, 1.0f);    // neutre

    add_biome("roche", "roche.png", true, true,
              0.65f, 0.3f, true, nullptr, 0.9f);    // plus haut et sec

    add_biome("montagne", "montagne.png", false, false,
              0.9f, 0.2f, true, nullptr, 1.05f);     // très haut et sec
}





Biome* BiomeManager::get(const std::string& name) {
    auto it = biome_map.find(name);
    return (it != biome_map.end() ? &it->second : &defaut);
}

Biome* BiomeManager::get_defaut_biome() {
    return &defaut;
}

std::vector<std::string> BiomeManager::get_liste_biome() {
    std::vector<std::string> names;
    for (const auto& [name, biome] : biome_map) {
        names.push_back(name);
    }
    return names;
}

std::vector<Biome*> BiomeManager::get_all_biome(){
    std::vector<Biome*> bio;
    for (auto& [name, biome] : biome_map) {
        bio.push_back(&biome);
    }
    return bio;
}

std::vector<Biome*> BiomeManager::get_all_natural_biome(){
    std::vector<Biome*> bio;
    for (auto& [name, biome] : biome_map) {
        if(biome.is_natural){
            bio.push_back(&biome);
        }
    }
    return bio;
}

void BiomeManager::clear() {
    biome_map.clear();
}

Biome* BiomeManager::get_best_biome(float altitude, float humidity) {
    Biome* best = &defaut;
    float best_score = -1.0f;

    for (auto& [name, biome] : biome_map) {
        float score = biome.match_score(altitude, humidity);
        if (score > best_score) {
            best = &biome;
            best_score = score;
        }
    }

    return best;
}
