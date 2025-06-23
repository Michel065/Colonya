#ifndef _HUB_STRUCTURE_H
#define _HUB_STRUCTURE_H

#include "../../Commun/includes.h"
#include "../../Tool/Stockage.h"
#include "../StructureManager.h"
#include "../../Map/Map.h"

class Hub : public Structure {
private:
    int taille_inv=5;
    Stockage stockage;

    mutable std::mutex mtx_entite;
    size_t capacite_entree = 3;
    std::vector<Entite*> entites_presentes;

    Map* carte=nullptr;


public:
    Hub(Map* map=nullptr, TimeManager* tm=nullptr);
    Hub(const Hub& other);

    void set_map(Map* map);
    void set_time_manager(TimeManager* tm);
    
    void update(Case& c) override;

    // pour le stockage
    bool ajouter(Ressource* value);
    bool retirer_type(RessourceType type, Ressource*& value);
    bool est_plein() const;
    std::vector<RessourceType> get_all_ressource() const;
    const Stockage* get_stockage() const; // c la mais pas touche
    
    // Interaction entité
    bool peut_entrer(Entite* e) const;
    bool entrer(Entite* e);
    void sortir(Entite* e);
    bool est_present(Entite* e) const;
    const std::vector<Entite*>& get_liste_des_entite_presente() const; // c la mais pas touche



    //focntion
    bool dormir(Entite* e);


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
