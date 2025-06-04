#ifndef _BIOME_H
#define _BIOME_H 

#include "../includes.h"

class BiomeManager;
struct Case;

struct Biome {
    std::string name;
    std::string texture;
    bool walkable=false;
    bool contructible=false;
    
    float altitude_min = 0.0f;
    float altitude_max = 1.0f;
    float humidity_min = 0.0f;
    float humidity_max = 1.0f;

    bool is_natural = true;

    std::function<void(Case&)> evolution_logic = nullptr;  // logique optionnelle

    void update(Case& c) const {
        if (evolution_logic) evolution_logic(c);
    }

    bool matches(float altitude, float humidity) const {// pour la generation
        return altitude >= altitude_min && altitude <= altitude_max &&
               humidity >= humidity_min && humidity <= humidity_max;
    }
};

//la seriralisation c dans le fichier biomemanager.h

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Biome& biome) {
    os << "[Biome: " << biome.name 
       << " | Texture: " << biome.texture 
       << " | Walkable: " << (biome.walkable ? "oui" : "non")
       << " | Contructible: " << (biome.contructible ? "oui" : "non")
       << "]";
    return os;
}

#endif
