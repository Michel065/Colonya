#ifndef _RESSOURCE_INFO_H
#define _RESSOURCE_INFO_H

#include "../Commun/includes.h"

struct RessourceInfo {
    std::string nom="Ressource inconue";
    int utilisation_base=1;
    std::string texture_path=textures_file+"/indefini.jpg";
};

#endif