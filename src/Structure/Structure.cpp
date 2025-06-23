#include "Structure.h"
#include "../Map/Case.h"

Structure::Structure(const Structure& other)
    : type(other.get_type()) {}

StructureType Structure::get_type() const {
    return type;
}

StructureState Structure::get_state()const{
    return state;
}

void Structure::set_state(StructureState sta){
    state=sta;
}

std::string Structure::get_texture(){
    return StructureInfoManager::get_info(type).texture_path;
}

std::string Structure::get_name()const{
    return (StructureInfoManager::get_info(type)).nom;
}

StructureInfo& Structure::get_info(){
    return StructureInfoManager::get_info(type);
}

void Structure::set_time_manager(TimeManager* time){
    time_manager=time;
}











Structure& Structure::operator=(const Structure& other) {
    type = other.get_type();
    set_from_structure(other);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Structure& r) {
    os << "[Structure: name=" << r.get_name() << r.get_print_string() << "]";
    return os;
}