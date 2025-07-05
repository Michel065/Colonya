#ifndef _MAP_H
#define _MAP_H

#include "../Commun/includes.h"
#include "./Chunk.h"

struct pair_hash {// merci chat
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class Map {
private:

    std::atomic<bool> jour=true; // pour savoir l'etat de la jouréne

    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> loaded_chunks;

    mutable std::shared_mutex mutex;

    std::pair<int, int> chunk_spawn={0,0};

public:

    std::pair<int, int> get_chunk_coords(int world_x, int world_y) const;
    std::pair<int, int> get_local_coords(int world_x, int world_y) const;
    std::pair<float, float> get_local_coords_float(int world_x, int world_y) const;
    
    Chunk * get_chunk(int chunk_x, int chunk_y);
    Case* get_case(int world_x, int world_y);
    void delete_chunk(Chunk* chu);
    void deload_chunk(int cx, int cy);
    void save_chunk(int chunk_x, int chunk_y);
    void save_all_chunks();
    bool chunk_deja_load(int chunk_x, int chunk_y);
    void load_chunk(int chunk_x, int chunk_y);
    void create_json_chunk(Chunk& chunk);
    std::vector<std::pair<int, int>>  get_all_chunk_keys();
    void load_chunk_from_liste(std::vector<std::pair<int, int>> chunks);
    void deload_chunk_from_liste(std::vector<std::pair<int, int>> chunks);
    void set_chunk_spawn(std::pair<int, int> chunk_spaw);
    std::pair<int, int> get_chunk_spawn();


    void print_chunks_load();


    bool il_fait_jour();
    void inverse_jour();
    
    void add_user_to_chunk(int chunk_x, int chunk_y);
    void supp_user_to_chunk(int chunk_x, int chunk_y);
};

#endif
