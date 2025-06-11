#ifndef _ENTITE_H
#define _ENTITE_H 

#include "../Commun/includes.h"

struct Entite {
    std::string name;

    std::atomic<int> faim=100;
    std::atomic<int> soif=100;
    std::atomic<int> sommeil=100;
    
    
    Entite(){};

};

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Entite& str) {
    os << "[Structure: " << str.name 
       << "]";
    return os;
}



#endif
