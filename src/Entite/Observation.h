#ifndef OBSERVATION_H
#define OBSERVATION_H 

#include "../Commun/includes.h"

struct Observation {
    std::string label;
    bool walkable;
    int x=0;
    int y=0;
    void complete_coord(int xx,int yy){x=xx;y=yy;}
};

#endif