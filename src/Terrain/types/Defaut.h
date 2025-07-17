#ifndef _DEFAUT_H
#define _DEFAUT_H

#include "../Terrain.h"
#include "../TerrainManager.h"

class Defaut : public Terrain {
public:
    Defaut() {
        name = "defaut";
        texture = "defaut.png";
        walkable = false;
        contructible = false;
        alt_target = 0;
        hum_target = 0;
        is_natural = false;
        score_boost = 0;
    }

    void update(Case& c) const override {
        if(!c.get_ressource()){
            
        }
    }

    Terrain* clone() const override { return new Defaut(*this); }

private:
    static inline AutoRegisterTerrain<Defaut> _reg{"defaut"};

};

#endif
