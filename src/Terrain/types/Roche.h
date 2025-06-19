#ifndef _ROCHE_H
#define _ROCHE_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Roche : public Terrain {
public:
    Roche() {
        name = "roche";
        texture = "roche.png";
        walkable = true;
        contructible = true;
        alt_target = 0.65f;
        hum_target = 0.3f;
        is_natural = true;
        score_boost = 0.9f;
    }

    void update(Case& c) const override {}

    Terrain* clone() const override { return new Roche(*this); }

private:
    static inline AutoRegisterTerrain<Roche> _reg{"roche"};

};

#endif
