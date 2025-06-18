#ifndef _STOCKAGE_H
#define _STOCKAGE_H

#include "../Commun/includes.h"
#include "../Ressource/all_ressource.h"

class Stockage {
private:
    std::deque<Ressource*> data;
    mutable std::mutex mtx;
    size_t  capacite_max;

public:
    explicit Stockage(int max = 5) : capacite_max(static_cast<size_t>(max)) {}

    bool ajouter(Ressource* value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (data.size() >= capacite_max) return false;
        data.push_back(value);
        return true;
    }

    bool retirer(Ressource*& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (data.empty()) return false;
        value = data.front();
        data.pop_front();
        return true;
    }

    bool est_plein() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data.size() >= capacite_max;
    }

    bool est_vide() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data.empty();
    }

    int taille() const {
        std::lock_guard<std::mutex> lock(mtx);
        return static_cast<int>(data.size());
    }

    int capacite() const {
        return static_cast<int>(capacite_max);
    }

    std::vector<Ressource*> snapshot() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<Ressource*> result;
        result.reserve(data.size());
        for (auto& r : data) {
            result.push_back(r);
        }
        return result;
    }

    std::vector<RessourceType> get_all_ressource() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::vector<RessourceType> result;
        result.reserve(data.size());
        for (auto& r : data) {
            result.push_back(r->get_type());
        }
        return result;
    }

    void clear(bool lok=true) {
        if(lok)std::lock_guard<std::mutex> lock(mtx);
        data.clear();
    }

    friend void to_json(nlohmann::json& j, const Stockage& s);
    friend void from_json(const nlohmann::json& j, Stockage& s);

};


inline void to_json(nlohmann::json& j, const Stockage& s) {
    std::lock_guard<std::mutex> lock(s.mtx);
    j["capacite"] = s.capacite_max;
    j["data"] = nlohmann::json::array();
    for (const auto* r : s.data) {
        j["data"].push_back(*r); // utilise ton to_json(const Ressource&)
    }
}

inline void from_json(const nlohmann::json& j, Stockage& s) {
    std::lock_guard<std::mutex> lock(s.mtx);
    s.clear(false);
    j.at("capacite").get_to(s.capacite_max);
    for (const auto& rjson : j.at("data")) {
        Ressource* r = nullptr;
        from_json(rjson, r); // utilise ton from_json(json, Ressource*&)
        if (r) s.data.push_back(r);
    }
}

#endif
