#ifndef _CASE_H
#define _CASE_H 

#include "../Commun/includes.h"

#include "./BiomeManager.h"
#include "../Ressource/RessourceManager.h"
#include "../Structure/Structure.h"

class Case {
private:
    Biome* biome = nullptr;
    Ressource* ressource = nullptr;
    Structure* structure = nullptr;
    bool constructible=true;
public:
    Case();
    ~Case();

    void set_all(Biome* bio, Ressource* re, Structure* st);

    void set_biome(Biome* bio);
    bool set_ressource(Ressource* bio,bool force=false);
    bool set_structure(Structure* bio,bool force=false);

    Biome* get_biome()const;
    Ressource* get_ressource()const;
    Structure* get_structure()const;
    Ressource* transfert_ressource();// donc plus dans case et recup entiere de la creature
        
    void delete_ressource();
    void delete_structure();
    void delete_biome();

    void update();

    Case* clone() const;
};

// JSON serialization
inline void to_json(json& j, const Case& c) {
    j["biome"] = c.get_biome();
    
    Ressource * tmp=c.get_ressource();
    if (tmp) j["ressource"] = *tmp;
    else j["ressource"] = nullptr;

    Structure* tmp2=c.get_structure();
    if (tmp2) j["structure"] = *tmp2;
    else j["structure"] = nullptr;
}

inline void from_json(const json& j, Case& c) {
    Biome* b = nullptr;
    j.at("biome").get_to(b);
    c.set_biome(b); 

    if (j.contains("ressource") && !j["ressource"].is_null()){
        Ressource* r=nullptr;
        from_json(j.at("ressource"), r);
        c.set_ressource(r,true);
    }

    if (j.contains("structure") && !j["structure"].is_null())
        c.set_structure(new Structure(j.at("structure").get<Structure>()),true);
}

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Case& casde) {
    os << "[Case: " 
       << " | " << *casde.get_biome() 
       << " | " << *casde.get_ressource()
       << " | " << *casde.get_structure()
       << "]";
    return os;
}

#endif
