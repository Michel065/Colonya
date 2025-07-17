#ifndef _EAU_RESSOURCE_H
#define _EAU_RESSOURCE_H

#include "../RessourceManager.h"

class Eau : public Ressource {
public:
    Eau();

    void consommer(Entite& ent) override;
    void update(Case& c) override;
    Ressource* clone() const override;

    static AutoRegisterRessource<Eau> reg;
    static AutoRegisterInfo<Eau> info;
};

#endif
