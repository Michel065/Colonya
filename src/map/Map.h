#ifndef _MAP_H
#define _MAP_H 

#include "../includes.h"

class Map {    
private:
    std::vector<int> chucks;
    std::string name_map;
    

public:
    Map(){}

    std::vector<std::string> getchuncksload(){
        std::vector<std::string> test;
        return test;
    }

    void load() {}

    void save() {}
};

#endif
