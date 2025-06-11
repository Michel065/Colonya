#include "Case.h"

Case::Case(){}
Case::~Case() {
    if(ressource)delete ressource;
    if(structure)delete structure;
}

void Case::set_all(Biome* bio, Ressource* re, Structure* st) {
    biome = bio;
    ressource = re;
    structure = st;
}

void Case::set_biome(Biome* bio) {
    biome = bio;
}

void Case::update() {
    if (biome) biome->update(*this);
    if (ressource) ressource->update_logique(*this);
    if (structure) structure->update(*this);
    constructible = biome && biome->contructible;
}

Case* Case::clone() const {
    Case* copy = new Case();
    copy->biome = biome; // partagé, pas copié
    copy->constructible = constructible;
    copy->ressource = ressource ? ressource->clone() : nullptr;
    copy->structure = structure ? structure->clone() : nullptr;
    return copy;
}