#ifndef _STRUCTURE_H
#define _STRUCTURE_H 

#include "../includes.h"
struct Case;

struct Structure {
    int name;
    
    void update(Case& c) const;
};

inline void to_json(json& j, const Structure& s) {
    j = json{{"type", s.name}};
}

inline void from_json(const json& j, Structure& s) {
    j.at("type").get_to(s.name);
}



#endif
