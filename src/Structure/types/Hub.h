#ifndef _HUB_STRUCTURE_H
#define _HUB_STRUCTURE_H

#include "../StructureManager.h"

class Hub : public Structure {
public:
    Hub() : Structure(StructureType::HUB, StructureInfoManager::get_info(StructureType::HUB).utilisation_base) {}

    void update(Case& c) override {
        // comportement spécifique, facultatif
    }

    Structure* clone() const override {
        return new Hub(*this);
    }

    // Auto-enregistrement
    static inline AutoRegisterStructure<Hub> reg{StructureType::HUB};
    static inline AutoRegisterStructureInfo<Hub> info{
        StructureType::HUB,
        StructureInfo{
            "Hub",
            textures_file + "maison.jpg" // pas utilisé pour l'instant mais si on veux ajouter un system de visualisation de case ou d'inventaire ca peux etre sympa
        }
    };
};

#endif
