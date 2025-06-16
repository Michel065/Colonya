#ifndef _MONTAGNE_H
#define _MONTAGNE_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Montagne : public Terrain {
public:
    Montagne() {
        name = "montagne";
        texture = "montagne.png";
        walkable = false;
        contructible = false;
        alt_target = 0.9f;
        hum_target = 0.2f;
        is_natural = true;
        score_boost = 1.05f;
    }

    void update(Case& c) const override {
        // logique spécifique si tu veux, sinon vide
    }

    Terrain* clone() const override { return new Montagne(*this); }

private:
    static inline AutoRegisterTerrain<Montagne> _reg{"montagne"};

};

#endif
