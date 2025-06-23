#ifndef _ENTITE_H
#define _ENTITE_H 

#include "../Commun/includes.h"

class Entite {
private:
    std::string name;
    int faim_max=100,soif_max=100,sommeil_max=100;

public:
    std::atomic<int> faim;
    std::atomic<int> soif;
    std::atomic<int> sommeil;
    
    
    Entite(std::string nam="Tom"):name(nam),faim(faim_max),soif(soif_max),sommeil(sommeil_max) {};

    std::string get_name() const{
        return name;
    }

    bool peux_dormir(){
        return (sommeil_max<=sommeil);
    }

    void dormir(int val){
        sommeil = std::min(sommeil+val,sommeil_max);
    }

};

// Surcharge de l'opérateur <<
inline std::ostream& operator<<(std::ostream& os, const Entite& str) {
    os << "[Structure: " << str.get_name()
       << "]";
    return os;
}



#endif
