#include "MapManager.h"

MapManager::MapManager(std::string name_map, TimeManager& tm, NoiseParam* param_generator,std::pair<int, int> chunk_spaw) : name_map(name_map),time_manager(&tm) {  
    world_file=worlds_file+name_map;
    chunk_spawn=chunk_spaw;
    if (fs::exists(world_file)) {
        print_primaire("Le monde ",world_file," existe.");
    } else {
        print_error("Le monde ",world_file," n'existe pas!");
        print_primaire("Creation du monde ...");
        print_status(true,"Creation du dossier monde");
        fs::create_directories(world_file);
        print_status(false,"Creation du dossier monde");
        init_map_contexte_file(param_generator);
    }
    load_map_contexte();
}

MapManager::~MapManager(){
    if(generateur)delete generateur;
}

void MapManager::init_map_contexte_file(NoiseParam* param_generator) {
    print_status(true,"Initialisation du contexte de generation de la carte");
    MapContexte contexte;
    contexte.chunk_spawn=chunk_spawn;
    if(!param_generator)contexte.param=new NoiseParam();
    else contexte.param=param_generator;

    json contexte_json = contexte; 
    std::ofstream file(world_file+"/"+world_contexte_name);
    file << contexte_json.dump(4);
    file.close();
    print_status(false,"Initialisation du contexte de generation de la carte");
}

void MapManager::save_map_contexte() {
    print_status(true,"sauvegarde map contexte");
    MapContexte contexte;
    contexte.chunks=carte.get_all_chunk_keys();
    contexte.chunk_spawn=carte.get_chunk_spawn();
    if(generateur)contexte.param=generateur->get_param();
    else contexte.param=new NoiseParam();

    json contexte_json = contexte; 
    std::ofstream file(world_file+"/"+world_contexte_name);
    file << contexte_json.dump(4);
    file.close();
    print_status(false,"sauvegarde map contexte");
}

void MapManager::load_map_contexte() {
    print_status(true,"load map contexte");
    std::string path=world_file+"/"+world_contexte_name;
    if (!fs::exists(path) || fs::is_empty(path)) {
        print("Aucun map contexte détecté. Initialisation de Map manager avec les valeur apr defaut.");
        generateur=new MapGenerator(new NoiseParam);
        return;
    }
    std::ifstream file(path);
    json contexte_json;
    file >> contexte_json;
    file.close();
    MapContexte contexte= contexte_json.get<MapContexte>();

    //cas ou tout va bien
    generateur=new MapGenerator(contexte.param,chunk_spawn);
    carte.set_chunk_spawn(contexte.chunk_spawn);
    load_chunk(contexte.chunk_spawn.first, contexte.chunk_spawn.second);
    load_all_chunk_from_liste(contexte.chunks);
    print_status(false,"load map contexte");
}

Map& MapManager::get_map(){
    return carte;
}

void MapManager::demander_load_chunk(int x, int y) {
    {
        std::lock_guard<std::mutex> lock(mtx_chunks);
        chunks_a_load_share.emplace_back(x, y);
    }
    time_manager->signal_event(); // réveille le thread si besoin
}

void MapManager::demander_deload_chunk(int x, int y) {
    {
        std::lock_guard<std::mutex> lock(mtx_chunks);
        chunks_a_deload_share.emplace_back(x, y);
    }
    time_manager->signal_event(); // réveille le thread si besoin
}

bool MapManager::chunk_existe(int x, int y){
    return fs::exists(world_file+"/"+std::to_string(x)+"x"+std::to_string(y)+".json");
}

void MapManager::create_chunk(int x, int y){
    if(!chunk_existe(x,y)){
        Chunk * ch =generateur->generate_chunk(x,y);
        carte.create_json_chunk(*ch);
    }
    else print_secondaire_attention("chunk "+std::to_string(x)+"x"+std::to_string(y)+" existe deja creation annule");

}

void MapManager::load_chunk(int x, int y){
    print_secondaire("load chunk ",x,"x",y);
    if(!chunk_existe(x,y)){
        create_chunk(x,y);
    }
    verif_et_creer_autour_chunk(x,y);
    carte.load_chunk(x,y);
}

void MapManager::load_all_chunk_from_liste(std::vector<std::pair<int, int>> chunks){
    for (const auto& [coord_x, coord_y] : chunks) {
        load_chunk(coord_x, coord_y);
    }
}

void MapManager::deload_all_chunk_from_liste(std::vector<std::pair<int, int>> chunks){
    carte.deload_chunk_from_liste(chunks);
}

void MapManager::verif_et_creer_autour_chunk(int xx, int yy){
    print_secondaire("verification autour chunk ",xx,"x",yy);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            if(!(x == 0 && y == 0))create_chunk(xx+x,yy+y);
        }
    }
}

void MapManager::start() {
    print_primaire("Demarage Map Manager!");
    while (time_manager->status()) {
        std::vector<std::pair<int, int>> chunks_a_load_local;
        std::vector<std::pair<int, int>> chunks_a_deload_local;
        {
            std::lock_guard<std::mutex> lock(mtx_chunks);
            std::swap(chunks_a_load_local, chunks_a_load_share);
            std::swap(chunks_a_deload_local, chunks_a_deload_share);
        }

        if(!chunks_a_load_local.empty())load_all_chunk_from_liste(chunks_a_load_local);
        if(!chunks_a_deload_local.empty())deload_all_chunk_from_liste(chunks_a_deload_local);

        if (time_manager->get_date() % cycle_jour_nuit == 0) {
            // cycle jour/nuit
        }
        time_manager->wait_condition_and_tick(cycle_jour_nuit, [&]() {
            std::lock_guard<std::mutex> lock(mtx_chunks);
            return !chunks_a_load_share.empty() || !chunks_a_deload_share.empty();
        });
    }
    save_map_contexte();
    print_primaire("STOP Map Manager!");
}

void MapManager::set_cycle_jour_nuit(int val){
    cycle_jour_nuit=val;
}
