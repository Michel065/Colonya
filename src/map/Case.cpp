#include "Case.h"

Case::Case(){}
Case::~Case() {
    if(ressource)delete ressource;
    if(structure)delete structure;
    terrain=nullptr;
    ressource=nullptr;
    structure=nullptr;
}

void Case::delete_ressource(){
    delete ressource;
    ressource=nullptr;
}

void Case::delete_structure(){
    delete structure;
    structure=nullptr;
}

void Case::delete_terrain(){
    delete terrain;
    terrain=nullptr;
}

void Case::set_all(Terrain* ter, Ressource* re, Structure* st) {
    terrain = ter;
    ressource = re;
    structure = st;
}

void Case::set_terrain(Terrain* ter) {
    terrain = ter;
}

bool Case::set_ressource(Ressource* res,bool force){
    if(force || (structure == nullptr && ressource == nullptr && constructible)){
        if(force)delete_ressource();
        ressource=res;
        return true;
    }else return false;
}

bool Case::set_structure(Structure* stru,bool force){
    if(force || (structure == nullptr && ressource == nullptr && constructible)){
        if(force)delete_structure();
        structure=stru;
        return true;
    }else return false;
}

Terrain* Case::get_terrain()const{
    if(terrain){
        return terrain;
    }else return nullptr;
}

Ressource* Case::get_ressource()const{
    if(ressource){
        return ressource;
    }else return nullptr;
}
    
Ressource* Case::transfert_ressource(){
    if(ressource){
        Ressource* tmp=ressource;
        ressource=nullptr;
        return tmp;
    }else return nullptr;
}

Structure* Case::get_structure()const{
    if(structure){
        return structure;
    }else return nullptr;
}



void Case::update() {
    if (ressource && ressource->doit_etre_supprimee())delete_ressource();

    if (terrain) terrain->update(*this);
    if (ressource) ressource->update(*this);
    if (structure) structure->update(*this);
    constructible = terrain && terrain->contructible;
}

Case* Case::clone() const {
    Case* copy = new Case();
    copy->terrain = terrain; // partagé, pas copié
    copy->constructible = constructible;
    copy->ressource = ressource ? ressource->clone() : nullptr;
    copy->structure = structure ? structure->clone() : nullptr;
    return copy;
}