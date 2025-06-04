#include "BiomeManager.h"

void BiomeManager::load_biomes() {
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


