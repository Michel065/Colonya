#ifndef MAP_CONTEXTE_H
#define MAP_CONTEXTE_H

#include "../includes.h"
struct NoiseParam;

struct MapContexte {
    std::vector<std::pair<int, int>> chuncks;
    std::pair<int, int> chunk_spawn = {0, 0};
    NoiseParam* param; 
};

inline void to_json(json& j, const MapContexte& m) {
    j = json{
        {"chuncks", m.chuncks}
    };
        j["chunk_spawn"] = m.chunk_spawn;

    if (m.param) {
        j["Map_generator_param"] = *m.param;
    }
}

inline void from_json(const json& j, MapContexte& m) {
    j.at("chuncks").get_to(m.chuncks);
    j.at("chunk_spawn").get_to(m.chunk_spawn);
    
    m.param = new NoiseParam();
    if (j.contains("Map_generator_param") && !j["Map_generator_param"].is_null()) {
        j.at("Map_generator_param").get_to(*m.param);
    }
}

#endif