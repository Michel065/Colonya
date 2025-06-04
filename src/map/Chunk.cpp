#include "./Chunk.h"

Chunk::Chunk(int cx,int cy){
    coord_x=cx;
    coord_y=cy;
}

Chunk::Chunk(){}


Case& Chunk::at(int x, int y) {
    return grid[x][y];
}

void Chunk::set_case(int x, int y, const Case& c) {
    if (is_inside(x,y)){
        grid[x][y] = c;
    }
}

void Chunk::update_all() {
    for (int x = 0; x < CHUNK_SIZE; ++x)
        for (int y = 0; y < CHUNK_SIZE; ++y)
            grid[x][y].update();
}

bool Chunk::is_inside(int x, int y){
    return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE;
}

std::pair<int, int> Chunk::get_name(){
    return {coord_x, coord_y};
}

void Chunk::set_name(int cx,int cy){
    coord_x=cx;
    coord_y=cy;
}