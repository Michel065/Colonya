#include "Map.h"

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

Chunk& Map::get_chunk(int chunk_x, int chunk_y) {                // a verif apres
    std::shared_lock lock(mutex); 
    auto it = loaded_chunks.find({chunk_x, chunk_y});
    if (it != loaded_chunks.end()) return it->second;
    lock.unlock();

    std::unique_lock write_lock(mutex);// pour delock la modif c fait a chaque foi mais pas utilisé c au cas ou
    return loaded_chunks[{chunk_x, chunk_y}];
}

Case& Map::get_case(int world_x, int world_y) {
    auto [chunk_x, chunk_y] = get_chunk_coords(world_x, world_y);
    auto [local_x, local_y] = get_local_coords(world_x, world_y);
    return *(get_chunk(chunk_x, chunk_y).at(local_x, local_y));
}

void Map::erase_chunk(int cx, int cy) {
    std::unique_lock lock(mutex);
    loaded_chunks.erase({cx, cy});
}

void Map::save_chunk(int world_x, int world_y) {
    auto [chunk_x, chunk_y] = get_chunk_coords(world_x, world_y);
    print("Sauvegarde du chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " ...");
    Chunk chunk(chunk_x,chunk_y);
    {
        std::shared_lock lock(mutex);
        chunk = get_chunk(chunk_x, chunk_y);
    }

    std::string path = world_file + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".json";
    if (!fs::exists(path)) {
        create_json_chunk(chunk);  // crée si absent
        return;
    }
    print("la");
    json chunk_json = chunk;
    print("la");
    std::ofstream file(path);
    if (!file.is_open()) {
        print("Erreur : impossible d’ouvrir " + path);
        return;
    }
    
    file << chunk_json.dump();
    file.close();
    print("Chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " sauvegardé !");
    erase_chunk(chunk_x, chunk_y);
}

void Map::save_all_chunks() {
    for (const auto& [coord_x, coord_y] : get_all_chunk_keys()) {
        save_chunk(coord_x, coord_y);
    }
}

void Map::load_chunk(int world_x, int world_y) {
    auto [chunk_x, chunk_y] = get_chunk_coords(world_x, world_y);
    print("load chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " ...");

    std::string path = world_file + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".json";

    if (!fs::exists(path) || fs::is_empty(path)) {
        print("Erreur lors du chargement du chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + " !!!");
        return;
    }
    std::ifstream file(path);
    if (!file) {
        print("Impossible d’ouvrir le fichier : " + path);
        return;
    }

    json chunk_json;
    file >> chunk_json;
    file.close();
    Chunk chunk;
    from_json(chunk_json, chunk);
    {
        std::unique_lock lock(mutex); // 🔒
        loaded_chunks[{chunk_x, chunk_y}] = std::move(chunk);
    }
    print("chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ", Done");
}


void Map::create_json_chunk(Chunk& chunk){
    auto [chunk_x, chunk_y] = chunk.get_name();
    std::string path = world_file + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".json";
    if (fs::exists(path)) return;  // Ne fait rien si déjà existant

    print("Création du chunk " + std::to_string(chunk_x) + "x" + std::to_string(chunk_y));
    json chunk_json = chunk;

    std::ofstream file(path);
    if (!file.is_open()) {
        print("Erreur : impossible de créer " + path);
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
