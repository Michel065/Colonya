#ifndef _CHUNK_H
#define _CHUNK_H

#include "../includes.h"
#include "Case.h"

class Chunk {
private:
    static const int WIDTH = 50;
    static const int HEIGHT = 50;

    Case grid[WIDTH][HEIGHT];

public:

    Case& at(int x, int y);
    void set(Case[][]& gr);
    void update_all();
    friend void to_json(json& j, const Chunk& c);
    friend void Case::from_json(const json& j, Chunk& c);

};

#endif
