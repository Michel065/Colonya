#ifndef _CASE_H
#define _CASE_H 

#include "../includes.h"

#include "./BiomeManager.h"
#include "../Ressource/Ressource.h"
#include "../Structure/Structure.h"

struct Case {
    Biome* biome = nullptr;
    Ressource* ressource = nullptr;
    Structure* structure = nullptr;

    bool constructible=true;

    Case(){}
    ~Case() {
        if(ressource)delete ressource;
        if(structure)delete structure;
    }

    void set_all(Biome* bio, Ressource* re, Structure* st) {
        biome = bio;
        ressource = re;
        structure = st;
    }

    void set_biome(Biome* bio) {
        biome = bio;
    }
    
    void update() {
        if (biome) biome->update(*this);
        if (ressource) ressource->update(*this);
        if (structure) structure->update(*this);
        constructible = biome && biome->contructible;
    }

    Case* clone() const {
        Case* copy = new Case();
        copy->biome = biome; // partagé, pas copié
        copy->constructible = constructible;
        copy->ressource = ressource ? ressource->clone() : nullptr;
        copy->structure = structure ? structure->clone() : nullptr;
        return copy;
    }
};

// JSON serialization
inline void to_json(json& j, const Case& c) {
    j["biome"] = c.biome;

    if (c.ressource)
        j["ressource"] = *c.ressource;
    else
        j["ressource"] = nullptr;

    if (c.structure)
        j["structure"] = *c.structure;
    else
        j["structure"] = nullptr;
}

inline void from_json(const json& j, Case& c) {
    j.at("biome").get_to(c.biome);

    if (c.ressource) { delete c.ressource; c.ressource = nullptr; }
    if (c.structure) { delete c.structure; c.structure = nullptr; }

    if (j.contains("ressource") && !j["ressource"].is_null())
        c.ressource = new Ressource(j.at("ressource").get<Ressource>());

    if (j.contains("structure") && !j["structure"].is_null())
        c.structure = new Structure(j.at("structure").get<Structure>());
}

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Case& casde) {
    os << "[Case: " 
       << " | " << *casde.biome 
       << " | " << *casde.ressource
       << " | " << *casde.structure
       << "]";
    return os;
}

#endif
