#include "Pomme.h"
#include "../../Entite/Entite.h"

Pomme::Pomme()
    : Ressource(RessourceType::POMME, RessourceInfoManager::get_info(RessourceType::POMME).utilisation_base) {}

void Pomme::consommer(Entite& ent) {
    decremente_utilisation();
    std::cout << "La créature mange une pomme (" << get_nbr_utilisation() << " restantes)\n";
    ent.manger(5);
}

void Pomme::update(Case& c) {}
Ressource* Pomme::clone() const {return new Pomme(*this);}

AutoRegisterRessource<Pomme> Pomme::reg{RessourceType::POMME};
AutoRegisterInfo<Pomme> Pomme::info{
    RessourceType::POMME,
    RessourceInfo{
        "Pomme",
        5,
        textures_file + "pomme.jpg"// toujous pas fait mais si j'ai le temps un jour ils en auront vraiment une
    }
};
