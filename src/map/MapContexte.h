#ifndef MAP_CONTEXTE_H
#define MAP_CONTEXTE_H

#include "../includes.h"

struct MapContexte {
    std::vector<std::string> chuncks;
};

inline void to_json(json& j, const MapContexte& m) {
    j = json{{"chuncks", m.chuncks}};
}

inline void from_json(const json& j, MapContexte& m) {
    j.at("chuncks").get_to(m.chuncks);
}

#endif