#ifndef _HUB_STRUCTURE_H
#define _HUB_STRUCTURE_H

#include "../../Commun/includes.h"
#include "../../Tool/Stockage.h"
#include "../StructureManager.h"

class Hub : public Structure {
private:
    int taille_inv=5;
    Stockage stockage;

public:
    Hub();
    Hub(const Hub& other);
    void update(Case& c) override;
    bool ajouter(Ressource* value);
    bool retirer_type(RessourceType type, Ressource*& value);
    bool est_plein() const;
    std::vector<RessourceType> get_all_ressource() const;
    nlohmann::json get_json()const override;
    void from_json(nlohmann::json json) override;
    void set_from_structure(const Structure& other) override;
    std::string get_print_string()const override;


    Structure* clone() const override {
        return new Hub();
    }

    // Auto-enregistrement
    static inline AutoRegisterStructure<Hub> reg{StructureType::HUB};
    static inline AutoRegisterStructureInfo<Hub> info{
        StructureType::HUB,
        StructureInfo{
            "Hub",
            textures_file + "maison.jpg"
        }
    };
};

#endif
