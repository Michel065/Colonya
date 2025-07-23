#include "Eau.h"
#include "../../Entite/Entite.h"

Eau::Eau()
    : Ressource(RessourceType::EAU, RessourceInfoManager::get_info(RessourceType::EAU).utilisation_base) {}

void Eau::consommer(Entite& ent) {
    decremente_utilisation();
    std::cout << "La créature boit de l'eau (" << get_nbr_utilisation() << " restantes)\n";
    ent.boire(5);
}

void Eau::update(Case& c) {}

Ressource* Eau::clone() const {return new Eau(*this);}

// Auto init
AutoRegisterRessource<Eau> Eau::reg{RessourceType::EAU};
AutoRegisterInfo<Eau> Eau::info{
    RessourceType::EAU,
    RessourceInfo{
        "eau",
        20,
        textures_file + "eau.jpg"
    }
};
