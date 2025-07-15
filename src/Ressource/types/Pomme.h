#ifndef _POMME_RESSOURCE_H
#define _POMME_RESSOURCE_H

#include "../RessourceManager.h"

class Pomme : public Ressource {
public:
    Pomme() : Ressource(RessourceType::POMME, RessourceInfoManager::get_info(RessourceType::POMME).utilisation_base) {}

    void consommer(Entite* ent) override {
        decremente_utilisation();
        std::cout << "La créature mange une pomme (" << get_nbr_utilisation() << " restantes)\n";
        ent->manger(5);
    }

    std::vector<Action> get_actions_disponibles(Entite& e) override {
        std::vector<Action> actions;

        actions.emplace_back(
            "consommer " + get_name(),
            [this](Entite& ent) {
                this->consommer(&ent);
            }
        );

        return actions;
    }

    void update(Case& c) override {
        // comportement spécifique optionnel
    }

    Ressource* clone() const override {
        return new Pomme(*this);
    }

    // Auto-enregistrement
    static inline AutoRegisterRessource<Pomme> reg{RessourceType::POMME};
    static inline AutoRegisterInfo<Pomme> info{
        RessourceType::POMME,
        RessourceInfo{
            "Pomme",
            5,
            textures_file + "pomme.jpg"
        }
    };
};

#endif
