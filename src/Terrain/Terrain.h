#ifndef _TERRAIN_H
#define _TERRAIN_H 

#include "../Commun/includes.h"
#include "../Ressource/RessourceType.h"

class TerrainManager;
struct Case;

class Terrain {
public:
    std::string name;
    std::string texture;
    bool walkable=false;
    bool contructible=false;
    float alt_target = 0.5f;
    float hum_target = 0.5f;
    float score_boost = 1.0f;
    bool is_natural = true;
    RessourceType ressource_par_defaut=RessourceType::AUCUNE;

    std::string terrain_apres_evolution="";

    Terrain(std::string n = "defaut", std::string tex = "default.png",
            bool walk = true, bool build = true)
        : name(n), texture(tex), walkable(walk), contructible(build) {}

    virtual ~Terrain() = default;

    virtual std::string get_terrain_apres_evolution()const {return "defaut";}
    virtual void update(Case& c) const {}  // méthode virtuelle, on override dans les types
    virtual Terrain* clone() const = 0;

    std::string get_name(){return name;}
    RessourceType& get_ressource_par_defaut(){return ressource_par_defaut;}

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
