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
    if(ressource)delete ressource;
    ressource=nullptr;
}

void Case::delete_structure(){
    if(structure)delete structure;
    structure=nullptr;
}

void Case::delete_terrain(){
    // delete terrain; ca c pas bien mais c pas important
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
    if(force || (structure == nullptr && ressource == nullptr && is_constructible())){
        if(force)delete_ressource();
        if(force)delete_structure();
        ressource=res;
        return true;
    }else return false;
}

bool Case::set_structure(Structure* stru,bool force){
    if(force || (structure == nullptr && ressource == nullptr && is_constructible())){
        if(force)delete_ressource();
        if(force)delete_structure();
        structure=stru;
        return true;
    }else return false;
}

void Case::set_terrain_doit_evoluer(bool val){
    terrain_doit_evoluer=val;
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

    if(terrain && terrain_doit_evoluer){
        terrain=TerrainManager::get(terrain->get_terrain_apres_evolution());
        terrain_doit_evoluer=false;
    }
}

Case* Case::clone() const {
    Case* copy = new Case();
    copy->terrain = terrain; // partagé, pas copié
    copy->ressource = ressource ? ressource->clone() : nullptr;
    copy->structure = structure ? structure->clone() : nullptr;
    return copy;
}

bool Case::is_constructible(){
    return terrain && terrain->contructible && structure==nullptr && ressource==nullptr;
}

bool Case::is_walkable(){
    return terrain && terrain->walkable && structure==nullptr && ressource==nullptr;
}

std::vector<Action> Case::get_actions_disponibles(){
    update();
    std::vector<Action> actions;

    // Si la case contient une ressource
    if (ressource) {
        auto a = ressource->get_actions_disponibles(ent);
        actions.insert(actions.end(), a.begin(), a.end());
    }

    // Si la case contient une structure
    if (structure) {
        auto a = structure->get_actions_disponibles(ent);
        actions.insert(actions.end(), a.begin(), a.end());
    }

    return actions;
}
