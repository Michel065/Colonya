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

    float match_score(float altitude, float humidity) const {
        float a_score = 1.0f - std::abs((altitude_min + altitude_max)/2 - altitude) / ((altitude_max - altitude_min)/2);
        float h_score = 1.0f - std::abs((humidity_min + humidity_max)/2 - humidity) / ((humidity_max - humidity_min)/2);
        return std::max(0.0f, (a_score + h_score) / 2.0f);  // Entre 0 et 1
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
