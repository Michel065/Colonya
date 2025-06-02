#ifndef _CASE_H
#define _CASE_H 

#include "../includes.h"

#include "./BiomeManager.h"
#include "../Ressource/Ressource.h"
#include "../Structure/Structure.h"

struct Case {
    Biome* biome;
    Ressource ressource;
    Structure structure;

    void update() {
        if (biome) biome->update(*this);
        ressource.update(*this);
        structure.update(*this);
    }
};

// JSON serialization
inline void to_json(json& j, const Case& c) {
    j = json{
        {"biome", c.biome},
        {"ressource", c.ressource},
        {"structure", c.structure}
    };
}

inline void from_json(const json& j, Case& c) {
    j.at("biome").get_to(c.biome);
    j.at("ressource").get_to(c.ressource);
    j.at("structure").get_to(c.structure);
}

#endif
