#ifndef _RESSOURCE_H
#define _RESSOURCE_H 

#include "../Commun/includes.h"
#include "./RessourceType.h"
#include "./RessourceInfoManager.h"
#include "../Entite/Entite.h"

struct Case;

class Ressource {
protected:
    RessourceType type;
    std::atomic<int> utilisations;
    virtual void update_perso(Case& c) = 0;

public:
    Ressource(RessourceType t, int uses = 1) : type(t), utilisations(uses) {}
    Ressource(const Ressource& other);
    virtual ~Ressource() = default;

    void decremente_utilisation();
    void set_utilisation(int val);
    RessourceType get_type()const;
    int get_nbr_utilisation()const;
    void update_logique(Case& c);

    //recup en rappor avec les info fixe
    std::string get_texture();
    std::string get_name()const;
    RessourceInfo& get_info();

    virtual void consommer(Entite* ent,Case& c) = 0;


    virtual Ressource* clone() const = 0;

    Ressource& operator=(const Ressource& other);
};

std::ostream& operator<<(std::ostream& os, const Ressource& r);
#endif
