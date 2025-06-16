#ifndef _STRUCTURE_MANAGER_H
#define _STRUCTURE_MANAGER_H

#include "Structure.h"

class StructureManager {
public:
    using StructureCreator = std::function<Structure*()>;

    static Structure* creer(StructureType type) {
        auto it = get_structures().find(type);
        if (it != get_structures().end()) return it->second();
        return nullptr;
    }

    static void register_Structure(StructureType type, StructureCreator creator) {
        get_structures()[type] = creator;
    }

private:
    inline static std::unordered_map<StructureType, StructureCreator> Structures;
    static std::unordered_map<StructureType, StructureCreator>& get_structures() {
        return Structures;
    }
};

template<typename T> 
struct AutoRegisterStructure {
    AutoRegisterStructure(StructureType type) {
        StructureManager::register_Structure(type, []() { return new T(); });
    }
};




inline void to_json(json& j, const Structure& r) {
    j = json{
        {"type", r.get_type()},
    };
}


inline void from_json(const json& j, Structure*& r) {
    StructureType type;
    j.at("type").get_to(type);
    r = StructureManager::creer(type);
}


#endif
