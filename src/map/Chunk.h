#ifndef _CHUNK_H
#define _CHUNK_H

#include "../includes.h"
#include "Case.h"


class Chunk {
private:
    std::atomic<int> nb_observateurs = 0;
public:
    int coord_x,coord_y;//coord du quoi en haut a gauche
    Case* grid[CHUNK_SIZE][CHUNK_SIZE]; // pour plus tard remetre ca en privée et du coup modif to et from json.


    Chunk(int cx,int cy);
    Chunk();
    ~Chunk();
    void init_grid();
    Case* at(int x, int y);
    void set_case(int x, int y, Case* c);
    bool is_inside(int x, int y);
    void update_all();
    std::pair<int, int> get_name();
    void set_name(int cx,int cy);

    void add_user();
    void supp_user();
    bool il_y_a_des_user()const;
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
            if (c.grid[x][y]) {
                row.push_back(*c.grid[x][y]);
            } else {
                row.push_back(nullptr);
            }
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
            if (!grid_json.at(x).at(y).is_null()) {
                c.grid[x][y] = new Case();
                grid_json.at(x).at(y).get_to(*c.grid[x][y]);
            } else {
                c.grid[x][y] = nullptr;
            }
        }
    }
}

#endif
