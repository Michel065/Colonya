#ifndef _RESSOURCE_H
#define _RESSOURCE_H 

#include "../Commun/includes.h"
#include "../Synchronisation/TimeManager.h"

#include "./RessourceType.h"
#include "./RessourceInfoManager.h"
#include "../Entite/Entite.h"

struct Case;

class Ressource {
protected:
    RessourceType type;
    std::atomic<int> utilisations;
    TimeManager* time_manager=nullptr;

public:
    Ressource(RessourceType t, int uses = 1) : type(t), utilisations(uses) {}
    Ressource(const Ressource& other);
    virtual ~Ressource() = default;

    void decremente_utilisation();
    void set_utilisation(int val);
    RessourceType get_type()const;
    int get_nbr_utilisation()const;
    void set_time_manager(TimeManager* time);

    //recup en rappor avec les info fixe
    std::string get_texture();
    std::string get_name()const;
    RessourceInfo& get_info();

    virtual void consommer(Entite* ent,Case& c) = 0;
    virtual bool doit_etre_supprimee() const;
    virtual void update(Case& c) = 0;




    virtual Ressource* clone() const = 0;

    Ressource& operator=(const Ressource& other);
};

std::ostream& operator<<(std::ostream& os, const Ressource& r);
#endif
