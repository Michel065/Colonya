#ifndef MAP_CONTEXTE_H
#define MAP_CONTEXTE_H

#include "../Commun/includes.h"
struct NoiseParam;

struct MapContexte {
    std::vector<std::pair<int, int>> chunks;
    std::pair<int, int> chunk_spawn = {0, 0};
    std::pair<int, int> coord_spawn = {0, 0};
    NoiseParam* param; 
};

inline void to_json(json& j, const MapContexte& m) {
    j = json{
        {"chunks", m.chunks}
    };
    j["chunk_spawn"] = m.chunk_spawn;
    j["coord_spawn"] = m.coord_spawn;
    
    print_primaire("on est la:",m.coord_spawn.first,"x",m.coord_spawn.second);


    if (m.param) {
        j["Map_generator_param"] = *m.param;
    }
}

inline void from_json(const json& j, MapContexte& m) {
    j.at("chunks").get_to(m.chunks);
    j.at("chunk_spawn").get_to(m.chunk_spawn);
    j.at("coord_spawn").get_to(m.coord_spawn);
    
    m.param = new NoiseParam();
    if (j.contains("Map_generator_param") && !j["Map_generator_param"].is_null()) {
        j.at("Map_generator_param").get_to(*m.param);
    }
}

#endif