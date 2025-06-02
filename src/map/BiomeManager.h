
#ifndef _BIOME_MANAGER_H
#define _BIOME_MANAGER_H 

#include "../includes.h"

#include "Biome.h"

class BiomeManager {
private:
    static inline std::unordered_map<std::string, Biome> biome_map;
    static inline Biome unknown = {"unknown", "default.png", true};

public:
    static void load_defaults();
    static Biome* get(const std::string& name);
    static std::vector<std::string> get_liste_biome();
};


//seriralisation de biome.h
inline void to_json(json& j, Biome* b) {
    if (b) {
        j = json{{"biome", b->name}};
    } else {
        j = json{{"biome", "unknown"}};
    }
}

inline void from_json(const json& j, Biome*& b) {
    std::string name;
    j.at("biome").get_to(name);
    b = BiomeManager::get(name);
}


#endif
