#include "Structure.h"
#include "../Map/Case.h"

Structure::Structure(const Structure& other)
    : type(other.get_type()) {}

StructureType Structure::get_type() const {
    return type;
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
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Structure& r) {
    os << "[Structure: type=" << r.get_name() << "]";
    return os;
}