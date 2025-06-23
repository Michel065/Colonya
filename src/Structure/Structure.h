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
    std::atomic<int> nbr_coup_avant_fin_construction;
    StructureState state;
    TimeManager* time_manager=nullptr;

public:
    Structure(StructureType t, int nbr_coup_avant_fin_constructio = 0, StructureState stat=StructureState::EN_CONSTRUCTION) : type(t), nbr_coup_avant_fin_construction(nbr_coup_avant_fin_constructio), state(stat) {}
    Structure(const Structure& other);
    virtual ~Structure() = default;

    StructureType get_type()const;
    StructureState get_state()const;
    void set_state(StructureState sta);
    void set_time_manager(TimeManager* time);

    //recup en rappor avec les info fixe
    std::string get_texture();
    std::string get_name()const;
    StructureInfo& get_info();
    
    virtual void update(Case& c) = 0;

    virtual nlohmann::json get_json()const = 0;
    virtual void from_json(nlohmann::json json) = 0;
    virtual void set_from_structure(const Structure& other) = 0;// a voir mais pas forcent utile a sup peux etre
    virtual std::string get_print_string()const = 0;


    virtual Structure* clone() const = 0;

    Structure& operator=(const Structure& other);
    friend std::ostream& operator<<(std::ostream& os, const Structure& r);
};
#endif
