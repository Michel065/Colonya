#ifndef _EAU_H
#define _EAU_H

#include "../RessourceManager.h"

class Eau : public Ressource {
public:
    Eau() : Ressource(RessourceType::EAU, RessourceInfoManager::get_info(RessourceType::EAU).utilisation_base) {}

    void consommer(Entite* ent,Case& c) override {
        decremente_utilisation();
        std::cout << "La créature boit de l'eau (" << get_nbr_utilisation() << " restantes)\n";

        update_logique(c);
    }

    void update_perso(Case& c) override {
        // comportement spécifique, facultatif
    }

    Ressource* clone() const override {
        return new Eau(*this);
    }

    // Auto-enregistrement
    static inline AutoRegisterRessource<Eau> reg{RessourceType::EAU};
    static inline AutoRegisterInfo<Eau> info{
        RessourceType::EAU,
        RessourceInfo{
            "Eau",
            20,
            textures_file + "eau.jpg" // pas utilisé pour l'instant mais si on veux ajouter un system de visualisation de case ou d'inventaire ca peux etre sympa
        }
    };
};

#endif
