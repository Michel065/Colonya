#ifndef _HERBE_H
#define _HERBE_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Herbe : public Terrain {
public:
    Herbe() {
        name = "herbe";
        texture = "herbe.png";
        walkable = true;
        contructible = true;
        alt_target = 0.5f;
        hum_target = 0.65f;
        is_natural = true;
        score_boost = 1.0f;
    }

    void update(Case& c) const override {}

    Terrain* clone() const override { return new Herbe(*this); }

private:
    static inline AutoRegisterTerrain<Herbe> _reg{"herbe"};

};

#endif
