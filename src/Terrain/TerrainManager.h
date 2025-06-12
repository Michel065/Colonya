
#ifndef _TERRAIN_MANAGER_H
#define _TERRAIN_MANAGER_H 

#include "../Commun/includes.h"

#include "Terrain.h"

class TerrainManager {
private:
    static inline std::unordered_map<std::string, Terrain> terrain_map;
    static inline Terrain defaut = {"defaut", "default.png", true};

public:
    static void add_terrain(const std::string& name,
                      const std::string& texture,
                      bool walkable,
                      bool constructible,
                      float alt_target,
                      float hum_target,
                      bool is_natural = true,
                      std::function<void(Case&)> evolution_logic = nullptr,
                      float score_boost = 1.0f);
    static void load_terrains();
    static Terrain* get(const std::string& name);
    static Terrain* get_defaut_terrain();
    static std::vector<std::string> get_liste_terrain();
    static std::vector<Terrain*> get_all_terrain();
    static std::vector<Terrain*> get_all_natural_terrain();
    static Terrain* get_best_terrain(float altitude, float humidity);
    static void clear();
};


//seriralisation de terrain.h
inline void to_json(json& j, Terrain* b) {
    if (b) {
        j = json{{"terrain_name", b->name}};
    } else {
        j = json{{"terrain_name", "defaut"}};
    }
}

inline void from_json(const json& j, Terrain*& b) {
    std::string name;
    j.at("terrain_name").get_to(name);
    b = TerrainManager::get(name);
}


#endif
