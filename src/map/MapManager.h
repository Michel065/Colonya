#ifndef _MAP_MANAGER_H
#define _MAP_MANAGER_H 

#include "../includes.h"

#include "./Map.h"
#include "./MapGenerator.h"
#include "../Synchronisation/TimeManager.h"
#include "./MapContexte.h"

class MapManager {
private:
    std::string name_map="";
    std::string world_contexte_name="contexte.json";
    Map carte;
    MapGenerator generateur;
    TimeManager* time_manager;

public:
    MapManager(std::string name_map,TimeManager& time_manager);
    Map& get_map();
    void save_map_contexte();
    void load_map_contexte();
    
    void start();

};

#endif
