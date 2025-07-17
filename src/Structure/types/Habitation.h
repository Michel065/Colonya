#ifndef _HABITATION_H
#define _HABITATION_H

#include "../Structure.h"
#include "../../Entite/Entite.h"

class Habitation : public Structure {
public:
    Habitation(StructureType t, int nbr_coup_avant_fin_constructio = 0, StructureState state = StructureState::EN_CONSTRUCTION)
            : Structure(t, nbr_coup_avant_fin_constructio, state) {}

    virtual bool peut_entrer(Entite* e) const = 0;
    virtual bool entrer(Entite* e) = 0;
    virtual void sortir(Entite* e) = 0;
    virtual bool dormir(Entite* e) = 0;
};

#endif
