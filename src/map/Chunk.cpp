#include "./Chunk.h"

Chunk::Chunk(int cx,int cy){
    coord_x=cx;
    coord_y=cy;
    init_grid();
}

Chunk::Chunk(){init_grid();}

Chunk::~Chunk() {
    for (auto& row : grid) {
        for (Case* c : row) {
            delete c;
        }
    }
}

void Chunk::init_grid(){
    for (int x = 0; x < CHUNK_SIZE; ++x)
        for (int y = 0; y < CHUNK_SIZE; ++y)
            grid[x][y] =nullptr;
}

Case* Chunk::at(int x, int y) {
    return grid[x][y];
}

void Chunk::set_case(int x, int y,Case* c) {
    if (is_inside(x,y)){
        if (grid[x][y]) delete grid[x][y];
        grid[x][y] = c;
    }
}

void Chunk::update_all() {
    for (int x = 0; x < CHUNK_SIZE; ++x)
        for (int y = 0; y < CHUNK_SIZE; ++y)
            grid[x][y]->update();
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

void Chunk::add_user() {
    /*
    print_primaire("la c bon");
    print_primaire("Add user appelé sur chunk à ", this);
    */
    nb_observateurs.fetch_add(1, std::memory_order_relaxed);
    //print_primaire("la c plus bon");

}

void Chunk::supp_user() {
    nb_observateurs.fetch_sub(1, std::memory_order_relaxed);
}

bool Chunk::il_y_a_des_user() const {
    return nb_observateurs.load(std::memory_order_relaxed) > 0;
}

void Chunk::print_chunk_coord()const{
    print_secondaire("coord Chunk:",coord_x,"x",coord_y);
}
