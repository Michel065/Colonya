#ifndef _STRUCTURE_H
#define _STRUCTURE_H 

#include "../includes.h"
struct Case;

struct Structure {
    int name;
    
    Structure(){};
    void update(Case& c){}

    Structure* clone() const {
        Structure* copy = new Structure();
        copy->name = name;
        return copy;
    }
};

inline void to_json(json& j, const Structure& s) {
    j = json{{"type", s.name}};
}

inline void from_json(const json& j, Structure& s) {
    j.at("type").get_to(s.name);
}

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Structure& str) {
    os << "[Structure: " << str.name 
       << "]";
    return os;
}



#endif
