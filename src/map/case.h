#ifndef _CASE_H
#define _CASE_H 

#include "../Commun/includes.h"

#include "../Terrain/TerrainManager.h"
#include "../Ressource/RessourceManager.h"
#include "../Structure/StructureManager.h"
#include "../Tool/Action.h"

class Case {
private:
    Terrain* terrain = nullptr;
    Ressource* ressource = nullptr;
    Structure* structure = nullptr;

    bool terrain_doit_evoluer=false;

public:
    Case();
    ~Case();

    void set_all(Terrain* ter, Ressource* re, Structure* st);

    void set_terrain(Terrain* ter);
    bool set_ressource(Ressource* ter,bool force=false);
    bool set_structure(Structure* ter,bool force=false);
    void set_terrain_doit_evoluer(bool val);

    Terrain* get_terrain()const;
    Ressource* get_ressource()const;
    Structure* get_structure()const;
    Ressource* transfert_ressource();// donc plus dans case et recup entiere de la creature
        
    void delete_ressource();
    void delete_structure();
    void delete_terrain();

    bool is_constructible();

    bool is_walkable();

    void update();

    std::vector<Action> get_actions_disponibles();

    Case* clone() const;
};

// JSON serialization
inline void to_json(json& j, const Case& c) {
    j["terrain"] = c.get_terrain();
    
    Ressource * tmp=c.get_ressource();
    if (tmp) j["ressource"] = *tmp;
    else j["ressource"] = nullptr;

    Structure* tmp2=c.get_structure();
    if (tmp2){ 
        print_primaire("case hub en theorie: il y a une structure");
        j["structure"] = *tmp2;
    }
    else j["structure"] = nullptr;
}

inline void from_json(const json& j, Case& c) {
    Terrain* b = nullptr;
    j.at("terrain").get_to(b);
    c.set_terrain(b); 

    if (j.contains("ressource") && !j["ressource"].is_null()){
        Ressource* r=nullptr;
        from_json(j.at("ressource"), r);
        c.set_ressource(r,true);
    }

    if (j.contains("structure") && !j["structure"].is_null()){
        Structure* r=nullptr;
        from_json(j.at("structure"), r);
        c.set_structure(r,true);
    }
}

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Case& casde) {
    os << "[Case: " 
       << " | " << *casde.get_terrain() 
       << " | " << *casde.get_ressource()
       << " | " << *casde.get_structure()
       << "]";
    return os;
}

#endif
