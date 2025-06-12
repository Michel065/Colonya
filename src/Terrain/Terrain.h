#ifndef _TERRAIN_H
#define _TERRAIN_H 

#include "../Commun/includes.h"

class TerrainManager;
struct Case;

struct Terrain {
    std::string name;
    std::string texture;
    bool walkable=false;
    bool contructible=false;
    
    float alt_target = 0.5f;
    float hum_target = 0.5f;
    float score_boost = 1.0f;

    bool is_natural = true;

    std::function<void(Case&)> evolution_logic = nullptr;  // logique optionnelle

    void update(Case& c) const {
        if (evolution_logic) evolution_logic(c);
    }

    float match_score(float altitude, float humidity) const {
        float a_score = 1.0f - std::abs(alt_target - altitude);
        float h_score = 1.0f - std::abs(hum_target - humidity);

        float score = std::max(0.0f, (a_score + h_score) / 2.0f);
        return std::clamp(score * score_boost, 0.0f, 1.0f);
    }


};

//la seriralisation c dans le fichier terrainmanager.h

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Terrain& terrain) {
    os << "[Terrain: " << terrain.name 
       << " | Texture: " << terrain.texture 
       << " | Walkable: " << (terrain.walkable ? "oui" : "non")
       << " | Constructible: " << (terrain.contructible ? "oui" : "non")
       << "]";
    return os;
}

#endif
