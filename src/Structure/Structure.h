#ifndef _STRUCTURE_H
#define _STRUCTURE_H 

#include "../Commun/includes.h"
#include "../Synchronisation/TimeManager.h"

#include "./StructureType.h"
#include "./StructureInfoManager.h"
#include "../Entite/Entite.h"

struct Case;

class Structure {
protected:
    StructureType type;
    std::atomic<int> utilisations;
    TimeManager* time_manager=nullptr;

public:
    Structure(StructureType t, int uses = 1) : type(t), utilisations(uses) {}
    Structure(const Structure& other);
    virtual ~Structure() = default;

    StructureType get_type()const;
    void set_time_manager(TimeManager* time);

    //recup en rappor avec les info fixe
    std::string get_texture();
    std::string get_name()const;
    StructureInfo& get_info();
    
    virtual void update(Case& c) = 0;




    virtual Structure* clone() const = 0;

    Structure& operator=(const Structure& other);
};

std::ostream& operator<<(std::ostream& os, const Structure& r);
#endif
