#include "./Chunk.h"

Case& Chunk::at(int x, int y) {
    return grid[x][y];
}

void Chunk::set(Case[][]& gr){
    grid=gr;
}

void Chunk::update_all() {
    for (int x = 0; x < WIDTH; ++x)
        for (int y = 0; y < HEIGHT; ++y)
            grid[x][y].update();
}

// JSON
friend void Chunk::to_json(json& j, const Chunk& c) {
    j = json::array();
    for (int x = 0; x < WIDTH; ++x) {
        json row = json::array();
        for (int y = 0; y < HEIGHT; ++y)
            row.push_back(c.grid[x][y]);
        j.push_back(row);
    }
}

friend void Chunk::from_json(const json& j, Chunk& c) {
    for (int x = 0; x < WIDTH; ++x)
        for (int y = 0; y < HEIGHT; ++y)
            j.at(x).at(y).get_to(c.grid[x][y]);
}