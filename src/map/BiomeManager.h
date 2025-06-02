
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
    static const Biome* get(const std::string& name);
    static const std::vector<std::string> get_liste_biome();
};


#endif
