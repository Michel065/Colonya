#ifndef _STOCKAGE_H
#define _STOCKAGE_H

#include "../Commun/includes.h"
#include "../Ressource/all_ressource.h"

class Stockage {
private:
    std::deque<Ressource*> data;
    size_t capacite_max;

public:
    explicit Stockage(int max = 5) : capacite_max(static_cast<size_t>(max)) {}

    bool ajouter(Ressource* value) {
        if (data.size() >= capacite_max) return false;
        data.push_back(value);
        return true;
    }

    bool retirer_type(RessourceType type, Ressource*& value) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if ((*it)->get_type() == type) {
                value = *it;
                data.erase(it);
                return true;
            }
        }
        value = nullptr;
        return false;
    }

    bool retirer(Ressource*& value) {
        if (data.empty()) return false;
        value = data.front();
        data.pop_front();
        return true;
    }

    bool est_plein() const {
        return data.size() >= capacite_max;
    }

    bool est_vide() const {
        return data.empty();
    }

    int taille() const {
        return static_cast<int>(data.size());
    }

    int capacite() const {
        return static_cast<int>(capacite_max);
    }

    std::vector<Ressource*> snapshot() const {
        std::vector<Ressource*> result;
        result.reserve(data.size());
        for (auto& r : data) {
            result.push_back(r);
        }
        return result;
    }

    std::vector<RessourceType> get_all_ressource() const {
        std::vector<RessourceType> result;
        result.reserve(data.size());
        for (auto& r : data) {
            result.push_back(r->get_type());
        }
        return result;
    }

    void clear() {
        data.clear();
    }

    void purge_ressources_supprimees() {
        auto it = data.begin();
        while (it != data.end()) {
            if ((*it)->doit_etre_supprimee()) {
                delete *it;
                it = data.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::vector<Action> get_all_action_ressource() {
        purge_ressources_supprimees();

        std::vector<Action> actions;
        for (auto* res : data) {
            if (res) {
                auto a = res->get_actions_disponibles();
                actions.insert(actions.end(), a.begin(), a.end());
            }
        }

        return actions;
    }








    std::string stringifie() const {
        std::string result = "[";
        for (auto& r : data) {
            result += r->get_name() + ", ";
        }
        result += "]";
        return result;
    }

    friend void to_json(nlohmann::json& j, const Stockage& s);
    friend void from_json(const nlohmann::json& j, Stockage& s);

    Stockage* clone() {
        Stockage* tmp = new Stockage(static_cast<int>(capacite_max));
        for (auto& r : data) {
            tmp->ajouter(r->clone());
        }
        return tmp;
    }

    void copy_from(const Stockage* tmp) {
        clear();
        for (auto& r : tmp->snapshot()) {
            ajouter(r->clone());
        }
    }
};

inline void to_json(nlohmann::json& j, const Stockage& s) {
    j["capacite"] = s.capacite_max;
    j["data"] = nlohmann::json::array();
    for (const auto* r : s.data) {
        j["data"].push_back(*r);
    }
}

inline void from_json(const nlohmann::json& j, Stockage& s) {
    s.clear();
    j.at("capacite").get_to(s.capacite_max);
    for (const auto& rjson : j.at("data")) {
        Ressource* r = nullptr;
        from_json(rjson, r);
        if (r) s.data.push_back(r);
    }
}

#endif
