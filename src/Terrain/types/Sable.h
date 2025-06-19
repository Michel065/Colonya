#ifndef _SABLE_H
#define _SABLE_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Sable : public Terrain {
public:
    Sable() {
        name = "sable";
        texture = "sable.png";
        walkable = true;
        contructible = true;
        alt_target = 0.25f;
        hum_target = 0.65f;
        is_natural = true;
        score_boost = 1.01f;
    }

    void update(Case& c) const override {}

    Terrain* clone() const override { return new Sable(*this); }

private:
    static inline AutoRegisterTerrain<Sable> _reg{"sable"};

};

#endif
