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
    }

    void update(Case& c) const override {
        // logique spécifique si tu veux, sinon vide
    }

    Terrain* clone() const override { return new Eau_Terrain(*this); }

private:
    static inline AutoRegisterTerrain<Eau_Terrain> _reg{"eau"};

};

#endif
