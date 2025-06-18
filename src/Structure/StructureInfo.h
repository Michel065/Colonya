#ifndef _STRUCTURE_INFO_H
#define STRUCTURE_INFO_H

#include "../Commun/includes.h"

struct StructureInfo {
    std::string nom="Structure inconue";
    std::string texture_path=textures_file+"/indefini.jpg";
    int nbr_de_base_de_coup_avant_fin_construction=0;
    int nbr_de_base_de_coup_avant_fin_destruction=0;
    StructureState state_de_base=StructureState::FONCTIONNEL;
};

#endif