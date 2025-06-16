#ifndef _PLAINE_H
#define _PLAINE_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Plaine : public Terrain {
public:
    Plaine() {
        name = "plaine";
        texture = "terre.png";
        walkable = true;
        contructible = true;
        alt_target = 0.4f;
        hum_target = 0.55f;
        is_natural = true;
        score_boost = 1.0f;
    }

    void update(Case& c) const override {
        // logique spécifique si tu veux, sinon vide
    }

    Terrain* clone() const override { return new Plaine(*this); }

private:
    static inline AutoRegisterTerrain<Plaine> _reg{"plaine"};

};

#endif
