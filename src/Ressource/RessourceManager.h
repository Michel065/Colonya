#ifndef _RESSOURCE_MANAGER_H
#define _RESSOURCE_MANAGER_H

#include "Ressource.h"

class RessourceManager {
public:
    using RessourceCreator = std::function<Ressource*()>;

    static Ressource* creer(RessourceType type) {
        auto it = get_ressources().find(type);
        if (it != get_ressources().end()) return it->second();
        return nullptr;
    }

    static void register_ressource(RessourceType type, RessourceCreator creator) {
        get_ressources()[type] = creator;
    }

private:
    inline static std::unordered_map<RessourceType, RessourceCreator> Ressources;
    static std::unordered_map<RessourceType, RessourceCreator>& get_ressources() {
        return Ressources;
    }
};

template<typename T> 
struct AutoRegisterRessource {
    AutoRegisterRessource(RessourceType type) {
        RessourceManager::register_ressource(type, []() { return new T(); });
    }
};




inline void to_json(json& j, const Ressource& r) {
    j = json{
        {"type", r.get_type()},
        {"utilisations", r.get_nbr_utilisation()}
    };
}


inline void from_json(const json& j, Ressource*& r) {
    RessourceType type;
    j.at("type").get_to(type);
    r = RessourceManager::creer(type);
    if (r && j.contains("utilisations"))
        r->set_utilisation(j["utilisations"]);
    else r->set_utilisation(1);
}


#endif
