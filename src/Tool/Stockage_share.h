#ifndef _STOCKAGE_SHARE_H
#define _STOCKAGE_SHARE_H

#include "Stockage.h"

class StockageShare {
private:
    Stockage stockage;
    mutable std::mutex mtx;

public:
    explicit StockageShare(int max = 5) : stockage(max) {}

    bool ajouter(Ressource* value) {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.ajouter(value);
    }

    bool retirer_type(RessourceType type, Ressource*& value) {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.retirer_type(type, value);
    }

    bool retirer(Ressource*& value) {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.retirer(value);
    }

    bool est_plein() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.est_plein();
    }

    bool est_vide() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.est_vide();
    }

    int taille() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.taille();
    }

    int capacite() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.capacite();
    }

    std::vector<Ressource*> snapshot() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.snapshot();
    }

    std::vector<RessourceType> get_all_ressource() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.get_all_ressource();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        stockage.clear();
    }

    void purge_ressources_supprimees() {
        std::lock_guard<std::mutex> lock(mtx);
        stockage.purge_ressources_supprimees();
    }

    std::vector<Action> get_all_action_ressource() {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.get_all_action_ressource();
    }

    std::string stringifie() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.stringifie();
    }

    StockageShare* clone() {
        StockageShare* tmp = new StockageShare(stockage.capacite());
        for (auto& r : stockage.snapshot()) {
            tmp->ajouter(r->clone());
        }
        return tmp;
    }

    void copy_from(const StockageShare* tmp) {
        clear();
        for (auto& r : tmp->snapshot()) {
            ajouter(r->clone());
        }
    }
    

    friend void to_json(nlohmann::json& j, const StockageShare& s);
    friend void from_json(const nlohmann::json& j, StockageShare& s);
};

inline void to_json(nlohmann::json& j, const StockageShare& s) {
    std::lock_guard<std::mutex> lock(s.mtx);
    to_json(j, s.stockage);
}

inline void from_json(const nlohmann::json& j, StockageShare& s) {
    std::lock_guard<std::mutex> lock(s.mtx);
    from_json(j, s.stockage);
}


#endif
