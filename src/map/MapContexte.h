#ifndef MAP_CONTEXTE_H
#define MAP_CONTEXTE_H

#include "../includes.h"

struct MapContexte {
    std::vector<std::string> chuncks;
};

void to_json(json& j, const MapContexte& m);
void from_json(const json& j, MapContexte& m);

#endif