#ifndef _HUB_STRUCTURE_H
#define _HUB_STRUCTURE_H

#include "../StructureManager.h"

class Hub : public Structure {
private:
    
public:
    Hub() : Structure(StructureType::HUB,
                      StructureInfoManager::get_info(StructureType::HUB).nbr_de_base_de_coup_avant_fin_construction,
                      StructureInfoManager::get_info(StructureType::HUB).state_de_base) {}


    void update(Case& c) override {
        // comportement spécifique, facultatif
    }

    
    
    nlohmann::json get_json()const override {

    }
    
    void from_json(nlohmann::json json) override{

    } 

    void set_from_structure(const Structure& other) override{
        
    }

    
    std::string get_print_string()const override{
        return "";
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
            textures_file + "maison.jpg", // pas utilisé pour l'instant mais si on veux ajouter un system de visualisation de case ou d'inventaire ca peux etre sympa
            0;
        }
    };
};

#endif
