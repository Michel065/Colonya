#ifndef _CHUNK_H
#define _CHUNK_H

#include "../includes.h"
#include "Case.h"


class Chunk {
private:
public:
    int coord_x,coord_y;//coord du quoi en haut a gauche
    Case grid[CHUNK_SIZE][CHUNK_SIZE]; // pour plus tard remetre ca en privée et du coup modif to et from json.


    Chunk(int cx,int cy);
    Chunk();
    Case& at(int x, int y);
    void set_case(int x, int y, const Case& c);
    bool is_inside(int x, int y);
    void update_all();
    //friend void to_json(json& j, const Chunk& c);
    //friend void from_json(const json& j, Chunk& c);
    std::pair<int, int> get_name();
    void set_name(int cx,int cy);
};

inline void to_json(json& j, const Chunk& c) {
    j = {
        {"coord_x", c.coord_x},
        {"coord_y", c.coord_y},
        {"grid", json::array()}
    };

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        json row = json::array();
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            row.push_back(c.grid[x][y]);
        }
        j["grid"].push_back(row);
    }
}

inline void from_json(const json& j, Chunk& c) {
    c.coord_x = j.at("coord_x").get<int>();
    c.coord_y = j.at("coord_y").get<int>();
    const auto& grid_json = j.at("grid");

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            grid_json.at(x).at(y).get_to(c.grid[x][y]);
        }
    }
}

#endif
