
#ifndef _BIOME_MANAGER_H
#define _BIOME_MANAGER_H 

#include "../includes.h"

#include "Biome.h"

class BiomeManager {
private:
    static inline std::unordered_map<std::string, Biome> biome_map;
    static inline Biome defaut = {"defaut", "default.png", true};

public:
    static void load_biomes();
    static Biome* get(const std::string& name);
    static Biome* get_defaut_biome();
    static std::vector<std::string> get_liste_biome();
    static std::vector<Biome*> get_all_biome();
    static std::vector<Biome*> get_all_natural_biome();
};


//seriralisation de biome.h
inline void to_json(json& j, Biome* b) {
    if (b) {
        j = json{{"biome_name", b->name}};
    } else {
        j = json{{"biome_name", "defaut"}};
    }
}

inline void from_json(const json& j, Biome*& b) {
    std::string name;
    j.at("biome_name").get_to(name);
    b = BiomeManager::get(name);
}


#endif
