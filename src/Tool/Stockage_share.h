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

    std::string stringifie() const {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.stringifie();
    }

    void purge_ressources_supprimees() {
        std::lock_guard<std::mutex> lock(mtx);
        stockage.purge_ressources_supprimees();
    }

    std::vector<Action> get_all_action_ressource(Entite& ent) {
        std::lock_guard<std::mutex> lock(mtx);
        return stockage.get_all_action_ressource(ent);
    }
};


#endif
