#ifndef _POMME_RESSOURCE_H
#define _POMME_RESSOURCE_H

#include "../RessourceManager.h"

class Pomme : public Ressource {
public:
    Pomme();

    void consommer(Entite& ent) override;
    void update(Case& c) override;
    Ressource* clone() const override;

    static AutoRegisterRessource<Pomme> reg;
    static AutoRegisterInfo<Pomme> info;
};

#endif
