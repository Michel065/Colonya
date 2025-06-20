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




inline void to_json(json& j, const Structure& s) {
    j = json{
        {"type", s.get_type()},
        {"data", s.get_json()},
    };
}


inline void from_json(const json& j, Structure*& s) {
    StructureType type;
    j.at("type").get_to(type);
    s = StructureManager::creer(type);
    if (s && j.contains("data"))
        s->from_json(j["data"]);

}


#endif
