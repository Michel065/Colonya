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

    bool constructible=true;

    void set(Biome* bio, Ressource re,Structure st){biome=bio;ressource=re;structure=st;}
    void update() {
        if (biome) biome->update(*this);
        ressource.update(*this);
        structure.update(*this);
        
        constructible=biome->contructible;// pas utile a revoir

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

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Case& casde) {
    os << "[Case: " 
       << " | " << *casde.biome 
       << " | " << casde.ressource
       << " | " << casde.structure
       << "]";
    return os;
}

#endif
