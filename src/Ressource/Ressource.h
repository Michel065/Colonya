#ifndef _RESSOURCE_H
#define _RESSOURCE_H 

#include "../includes.h"
struct Case;


/*
les ressource ne sont pas cumulable (ex:64) par contre elle on une quantité d'utilisation (ex: eau 20 utilisation)
*/

struct Ressource {
    int name;
    //std::string nom_ressource;
    //int quantite;
    Ressource(){};

    void update(Case& c){}
    //void consome(Creature c) ; comme ca ca modif direct les sta de la creature si c un buff ou de l'eau du mangé ... 
    //quand cide s'autodetruit

    Ressource* clone() const {
        Ressource* copy = new Ressource();
        copy->name = name;
        return copy;
    }
};

inline void to_json(json& j, const Ressource& r) {
    j = json{{"name", r.name}};
}

inline void from_json(const json& j, Ressource& r) {
    j.at("name").get_to(r.name);
}

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Ressource& res) {
    os << "[Ressource: " << res.name 
       << "]";
    return os;
}

#endif
