#include "MapManager.h"

MapManager::MapManager(std::string name_map, TimeManager& tm) : name_map(name_map),time_manager(&tm) {  
    world_file=worlds_file+name_map;
    print(world_file);
    if (fs::exists(world_file)) {
        print("Le monde existe.");
        load_map_contexte();
    } else {
        print("Le monde n'existe pas! \n Creation en cours ...");
        fs::create_directories(world_file); 
        print("Dossier creer!");
        save_map_contexte();
    }
}

void MapManager::save_map_contexte() {
    print("save contexte ...");
    MapContexte contexte;
    contexte.chuncks=carte.getchuncksload();
    json contexte_json = contexte; 
    std::ofstream file(world_file+"/"+world_contexte_name);
    file << contexte_json.dump(4);  // 4 = indentation
    file.close();
    print("contexte save !");
}

void MapManager::load_map_contexte() {
    print("load contexte ...");
    std::string path=world_file+"/"+world_contexte_name;

    if (!fs::exists(path) || fs::is_empty(path)) {
        print("Aucun contexte détecté. Initialisation d’un fichier vide.");
        MapContexte contexte;
        contexte.chuncks = {};
        std::ofstream file(path);
        file << json(contexte).dump(4);
        file.close();
        return;
    }

    std::ifstream file(path);
    json contexte_json;
    file >> contexte_json;
    file.close();
    MapContexte contexte= contexte_json.get<MapContexte>();

    //ensuite faut mettre ka logique
    print("contexte load !");
}

void MapManager::start() {
    while (time_manager->status()) {
        time_manager->waitNextTick();
    }
}