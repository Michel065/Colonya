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

    bool retirer_type(RessourceType type, Ressource*& value) {
        std::lock_guard<std::mutex> lock(mtx);
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

    std::string stringifie() const{
        std::lock_guard<std::mutex> lock(mtx);
        std::string result="[";
        for (auto& r : data) {
            result+=r->get_name()+", ";
        }
        result+="]";
        return result;
    }

    friend void to_json(nlohmann::json& j, const Stockage& s);
    friend void from_json(const nlohmann::json& j, Stockage& s);

    Stockage* clone(){
        Stockage* tmp=new Stockage(static_cast<int>(capacite_max));
        for (auto& r : data) {
            tmp->ajouter(r->clone());
        }
        return tmp;
    }

    void copy_from(const Stockage* tmp){
        clear();
        for (auto& r : tmp->snapshot()) {
            ajouter(r->clone());
        }
    }
};


inline void to_json(nlohmann::json& j, const Stockage& s) {
    std::lock_guard<std::mutex> lock(s.mtx);
    j["capacite"] = s.capacite_max;
    j["data"] = nlohmann::json::array();
    for (const auto* r : s.data) {
        j["data"].push_back(*r);
    }
}

inline void from_json(const nlohmann::json& j, Stockage& s) {
    std::lock_guard<std::mutex> lock(s.mtx);
    s.clear(false);
    j.at("capacite").get_to(s.capacite_max);
    for (const auto& rjson : j.at("data")) {
        Ressource* r = nullptr;
        from_json(rjson, r);
        if (r) s.data.push_back(r);
    }
}

#endif
