#ifndef _RESSOURCE_H
#define _RESSOURCE_H 

#include "../includes.h"
struct Case;

struct Ressource {
    int name;

    void update(Case& c) const;
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
