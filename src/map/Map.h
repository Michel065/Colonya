#ifndef _MAP_H
#define _MAP_H

#include "../includes.h"
#include "./Chunk.h"

struct pair_hash {// merci chat
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class Map {
private:
    std::unordered_map<std::pair<int, int>, Chunk, pair_hash> loaded_chunks;
    mutable std::shared_mutex mutex;

    std::pair<int, int> get_chunk_coords(int world_x, int world_y) const;
    std::pair<int, int> get_local_coords(int world_x, int world_y) const;

public:
    Chunk& get_chunk(int chunk_x, int chunk_y);
    Case& get_case(int world_x, int world_y);
    void set_case(int world_x, int world_y, const Case& c);
    void erase_chunk(int cx, int cy);
    void save_chunk(int world_x, int world_y);
    void save_all_chunks();
    void load_chunk(int world_x, int world_y);
    void create_json_chunk(Chunk& chunk);
    std::vector<std::pair<int, int>>  get_all_chunk_keys();
};

#endif
