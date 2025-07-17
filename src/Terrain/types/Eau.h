#ifndef _EAU_TERRAIN_H
#define _EAU_TERRAIN_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Eau_Terrain : public Terrain {
public:
    Eau_Terrain() {
        name = "eau";
        texture = "eau.png";
        walkable = false;
        contructible = false;
        alt_target = 0.15f;
        hum_target = 0.85f;
        is_natural = true;
        score_boost = 1.2f;
        ressource_par_defaut=RessourceType::EAU;
        terrain_apres_evolution="sable";
    }

    std::string get_terrain_apres_evolution() const override {
        return terrain_apres_evolution;
    }

    void update(Case& c) const override {
        if(!c.get_ressource()){
            c.set_terrain_doit_evoluer(true);
        }
    }

    Terrain* clone() const override { return new Eau_Terrain(*this); }

private:
    static inline AutoRegisterTerrain<Eau_Terrain> _reg{"eau"};

};

#endif
