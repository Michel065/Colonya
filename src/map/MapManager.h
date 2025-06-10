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
    MapGenerator* generateur;
    TimeManager* time_manager;

    int cycle_jour_nuit = 50;
    
    std::vector<std::pair<int, int>> chunks_a_load_share;
    std::vector<std::pair<int, int>> chunks_a_deload_share;
    std::pair<int, int> chunk_spawn = {0, 0};

    std::mutex mtx_chunks;

public:
    MapManager(std::string name_map,TimeManager& time_manager, NoiseParam* param_generator=nullptr ,std::pair<int, int> chunk_spaw = {0, 0});
    ~MapManager();
    Map& get_map();
    void init_map_contexte_file(NoiseParam* param_generator=nullptr);
    void save_map_contexte();
    void load_map_contexte();
    void demander_load_chunk(int x, int y);
    void demander_deload_chunk(int x, int y);

    bool chunk_existe(int x, int y);
    void create_chunk(int x, int y);
    void load_chunk(int x, int y);
    void load_all_chunk_from_liste(std::vector<std::pair<int, int>> chunks);
    void deload_all_chunk_from_liste(std::vector<std::pair<int, int>> chunks);
    void verif_et_creer_autour_chunk(int x, int y);

    void set_cycle_jour_nuit(int val);
    
    void start();

};

#endif
