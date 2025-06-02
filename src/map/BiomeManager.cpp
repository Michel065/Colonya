#include "BiomeManager.h"

void BiomeManager::load_defaults() {
    //basique
    biome_map["terre"]     = {"terre",     textures_file + "terre.png",     true,   true};
    biome_map["herbe"]     = {"herbe",     textures_file + "herbe.png",     true,   true};
    biome_map["roche"]     = {"roche",     textures_file + "roche.png",     true,   true};
    biome_map["montagne"]  = {"montagne",  textures_file + "montagne.png",  false,   false};
    biome_map["eau"] = {"eau", textures_file + "eau.png", false,   false};
    //biome_map["eau_assechee"] = {"eau_assechee", textures_file + "eau_assechee.png", true,  true};


    //logique evolutions
    /*biome_map["eau"].evolution_logic = [](Case& c) { // a revoir quand la ressource eau sera faite
        if (c.ressource.quantite == 0) {
            c.biome = BiomeManager::get("eau_assechee");
        }
    };*/

}

Biome* BiomeManager::get(const std::string& name) {
    auto it = biome_map.find(name);
    return (it != biome_map.end() ? &it->second : &unknown);
}

std::vector<std::string> BiomeManager::get_liste_biome() {
    std::vector<std::string> names;
    for (const auto& [name, biome] : biome_map) {
        names.push_back(name);
    }
    return names;
}

