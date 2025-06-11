#ifndef _CASE_H
#define _CASE_H 

#include "../Commun/includes.h"

#include "./BiomeManager.h"
#include "../Ressource/RessourceManager.h"
#include "../Structure/Structure.h"

struct Case {
    Biome* biome = nullptr;
    Ressource* ressource = nullptr;
    Structure* structure = nullptr;

    bool constructible=true;

    Case();
    ~Case();

    void set_all(Biome* bio, Ressource* re, Structure* st);
    void set_biome(Biome* bio);
    bool set_ressource(Ressource* bio); // a finir
    bool set_structure(Structure* bio);

    Biome* get_biome();
    Ressource* get_ressource();
    Structure* get_structure();
    
    void update();

    Case* clone() const;
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
        from_json(j.at("ressource"), c.ressource);
    else
        delete c.ressource;


    if (j.contains("structure") && !j["structure"].is_null())
        c.structure = new Structure(j.at("structure").get<Structure>());
}

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Case& casde) {
    os << "[Case: " 
       << " | " << *casde.biome 
       << " | " << *(casde.ressource)
       << " | " << *casde.structure
       << "]";
    return os;
}

#endif
