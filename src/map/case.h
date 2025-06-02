#ifndef _CASE_H
#define _CASE_H 

#include "../includes.h"

#include "./Biome.h"
#include "../Ressource/Ressource.h"
#include "../Structure/Structure.h"

struct Case {
    Biome* biome;
    Ressource ressource;
    Structure structure;

    void update() {
        if (biome) biome->update(*this);
        ressource.update(*this);
        structure.update(*this);
    }
};


#endif
