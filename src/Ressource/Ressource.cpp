#include "Ressource.h"
#include "../Map/Case.h"

Ressource::Ressource(const Ressource& other)
    : type(other.get_type()), utilisations(other.get_nbr_utilisation()) {}

void Ressource::decremente_utilisation(){
    utilisations--;
}

void Ressource::set_utilisation(int val){
    utilisations.store(val);
}

RessourceType Ressource::get_type() const {
    return type;
}

int Ressource::get_nbr_utilisation()const{
    return utilisations.load();
}

bool Ressource::doit_etre_supprimee() const {
    return get_nbr_utilisation() <= 0;
}


std::string Ressource::get_texture(){
    return RessourceInfoManager::get_info(type).texture_path;
}

std::string Ressource::get_name()const{
    return (RessourceInfoManager::get_info(type)).nom;
}

RessourceInfo& Ressource::get_info(){
    return RessourceInfoManager::get_info(type);
}

void Ressource::set_time_manager(TimeManager* time){
    time_manager=time;
}











Ressource& Ressource::operator=(const Ressource& other) {
    type = other.get_type();
    utilisations.store(other.get_nbr_utilisation());
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Ressource& r) {
    os << "[Ressource: type=" << r.get_name() << ", utilisations=" << r.get_nbr_utilisation() << "]";
    return os;
}