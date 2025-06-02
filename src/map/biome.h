#ifndef _BIOME_H
#define _BIOME_H 

#include "../includes.h"

struct Biome {
    std::string name;
    std::string texture;
    bool walkable=false;
    bool contructible=false;
    
    std::function<void(Case&)> evolution_logic = nullptr;  // logique optionnelle

    void update(Case& c) const {
        if (evolution_logic) evolution_logic(c);
    }
};

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
