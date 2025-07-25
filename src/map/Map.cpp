#include "Map.h"

Map::~Map(){
    deload_all_chunk();
}

std::pair<int, int> Map::get_chunk_coords(int world_x, int world_y) const {
    int cx = world_x / CHUNK_SIZE;
    int cy = world_y / CHUNK_SIZE;
    if (world_x < 0) cx--;
    if (world_y < 0) cy--;
    return {cx, cy};
}

std::pair<int, int> Map::get_local_coords(int world_x, int world_y) const {
    int lx = ((world_x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int ly = ((world_y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    return {lx, ly};
}


std::pair<float, float> Map::get_local_coords_float(int world_x, int world_y) const{
    std::pair<int, int> coord = get_local_coords(world_x,world_y);
    float fx=0,fy=0;
    fx=static_cast<float>(coord.first)/CHUNK_SIZE;
    fy=static_cast<float>(coord.second)/CHUNK_SIZE;
    return {fx, fy};
}

Chunk* Map::get_chunk(int chunk_x, int chunk_y) {
    {
        std::shared_lock lock(mutex); 
        auto it = loaded_chunks.find({chunk_x, chunk_y});
        if (it != loaded_chunks.end()) {
            return it->second;
        }
    }

    std::unique_lock write_lock(mutex);
    auto it = loaded_chunks.find({chunk_x, chunk_y});
    if (it != loaded_chunks.end()) {
        return it->second;
    }
    return nullptr;
}


Case* Map::get_case(int world_x, int world_y) {
    auto [chunk_x, chunk_y] = get_chunk_coords(world_x, world_y);
    auto [local_x, local_y] = get_local_coords(world_x, world_y);
    return (get_chunk(chunk_x, chunk_y)->at(local_x, local_y));
}

void Map::delete_chunk(Chunk* chu) {
    if(chu)delete chu;
}

void Map::deload_chunk(int chunk_x, int chunk_y) {
    
    if (std::make_pair(chunk_x, chunk_y) == chunk_spawn) {
        print_secondaire_attention("Chunk de spawn protégé, non déchargé.");
        return;
    }
    
    std::lock_guard<std::shared_mutex> lock(mutex);
    auto it = loaded_chunks.find({chunk_x, chunk_y});
    if (it != loaded_chunks.end()) {
        Chunk* chunk = it->second;

        if (!chunk->il_y_a_des_user()) {
            loaded_chunks.erase(it);
            delete_chunk(chunk);
            print_secondaire("Chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " supprimé.");
        } else {
            print_secondaire_attention("Chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " encore utilisé. Suppression annulée.");
        }
    }
}

void Map::save_chunk(int chunk_x, int chunk_y) {
    print_secondaire("Sauvegarde du chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " ...");
    Chunk* chunk;
    {
        std::shared_lock lock(mutex);
        chunk = get_chunk(chunk_x, chunk_y);
    }

    std::string path = world_file + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".json";
    if (!fs::exists(path)) {
        create_json_chunk(*chunk);  // crée si absent
        return;
    }
    json chunk_json = *chunk;
    std::ofstream file(path);
    if (!file.is_open()) {
        print_error("Erreur : impossible d’ouvrir " + path);
        return;
    }
    
    file << chunk_json.dump();
    file.close();
    print_secondaire("Chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " sauvegardé !");
    deload_chunk(chunk_x, chunk_y);
}

void Map::save_all_chunks() {
    for (const auto& [coord_x, coord_y] : get_all_chunk_keys()) {
        save_chunk(coord_x, coord_y);
    }
}

bool Map::chunk_deja_load(int chunk_x, int chunk_y){
    {
        std::unique_lock lock(mutex);
        auto it = loaded_chunks.find({chunk_x, chunk_y});

        if (it != loaded_chunks.end()) {
            print_secondaire_attention("chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " déjà chargé");
            it->second->print_chunk_coord();
            return true;
        }
    }
    return false;
}

void Map::load_chunk(int chunk_x, int chunk_y) {// on av ajouter les gent quand il veul le load 
    print_secondaire("load chunk dans la carte " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " ...");
    print_chunks_load();
    if(chunk_deja_load(chunk_x, chunk_y))return;

    std::string path = world_file + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".json";

    if (!fs::exists(path) || fs::is_empty(path)) {
        print_error("Erreur lors du chargement du chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " !!!");
        return;
    }
    std::ifstream file(path);
    if (!file) {
        print_error("Impossible d’ouvrir le fichier : " + path);
        return;
    }

    json chunk_json;
    file >> chunk_json;
    file.close();
    Chunk* chunk = new Chunk;
    from_json(chunk_json, *chunk);
    {
        std::unique_lock lock(mutex);
        loaded_chunks[{chunk_x, chunk_y}] = chunk;
    }
    print_secondaire("chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ", Done");
}


void Map::create_json_chunk(Chunk& chunk){ // * maintennat
    auto [chunk_x, chunk_y] = chunk.get_name();
    std::string path = world_file + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".json";
    if (fs::exists(path)) return;  // Ne fait rien si déjà existant

    print_secondaire("Création du chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y));
    json chunk_json = chunk;

    std::ofstream file(path);
    if (!file.is_open()) {
        print_error("Erreur : impossible de créer " + path);
        return;
    }

    file << chunk_json.dump();
    file.close();
}

std::vector<std::pair<int, int>> Map::get_all_chunk_keys() {
    std::vector<std::pair<int, int>> keys;
    std::shared_lock lock(mutex);
    for (const auto& [coord, _] : loaded_chunks) {
        keys.push_back(coord);
    }
    return keys;
}

void Map::load_chunk_from_liste(std::vector<std::pair<int, int>> chunks){
    for (const auto& [coord_x, coord_y] : chunks) {
        load_chunk(coord_x, coord_y);
    }
}

void Map::deload_chunk_from_liste(std::vector<std::pair<int, int>> chunks){
    for (const auto& [coord_x, coord_y] : chunks) {
        deload_chunk(coord_x, coord_y);
    }
}

void Map::decharge_chunk_pas_utilise() {
    std::vector<std::pair<int, int>> a_supp;

    for (const auto& [coords, chunk] : loaded_chunks) {
        if (chunk && !chunk->il_y_a_des_user()) {
            a_supp.push_back(coords);
        }
    }

    for (const auto& [coord_x, coord_y] : a_supp) {
        deload_chunk(coord_x, coord_y);
    }
}

void Map::deload_all_chunk() {
    for (auto& [coord, chunk] : loaded_chunks) {
        if (chunk) delete chunk;
    }
    loaded_chunks.clear();
}


void Map::set_coord_spawn(std::pair<int, int> coord_spaw){
    coord_spawn=coord_spaw;
}

void Map::set_chunk_spawn(std::pair<int, int> chunk_spaw){
    chunk_spawn=chunk_spaw;
}

std::pair<int, int> Map::get_chunk_spawn(){
    return chunk_spawn;
}

std::pair<int, int> Map::get_coord_spawn()const{
    return coord_spawn;
}


void Map::print_chunks_load(){
    std::string texte="[";
    for (const auto& [coord, _] : loaded_chunks) {
        texte+="{"+std::to_string(coord.first)+"x"+std::to_string(coord.second)+"} ";
    }
    texte+="];";
    print_primaire(texte);
}



bool Map::il_fait_jour(){
    return jour.load();
}

void Map::inverse_jour(){
    jour.store(!jour.load());
}


void Map::add_user_to_chunk(int chunk_x, int chunk_y) {
    std::shared_lock lock(mutex); 
    auto it = loaded_chunks.find({chunk_x, chunk_y});
    if (it != loaded_chunks.end() && it->second) {
        it->second->add_user();
    } else {
        print_error("chunk présent dans loaded_chunks mais null : ", chunk_x, ", ", chunk_y);
    }
}


void Map::supp_user_to_chunk(int chunk_x, int chunk_y) {
    std::shared_lock lock(mutex); 
    auto it = loaded_chunks.find({chunk_x, chunk_y});
    if (it != loaded_chunks.end()) {
        it->second->supp_user();
    }
}
