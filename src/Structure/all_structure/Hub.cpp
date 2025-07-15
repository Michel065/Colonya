#include "Hub.h"
#include "../../Simu/Simu.h"

Hub::Hub(Map* map, TimeManager* tm) 
    : Structure(StructureType::HUB,
                StructureInfoManager::get_info(StructureType::HUB).nbr_de_base_de_coup_avant_fin_construction,
                StructureInfoManager::get_info(StructureType::HUB).state_de_base),
      stockage(taille_inv) {
    carte = map;
    time_manager = tm;
}

void Hub::set_map(Map* map) {
    carte = map;
}

void Hub::set_time_manager(TimeManager* tm) {
    time_manager = tm;
}

void Hub::update(Case& c) {}

bool Hub::ajouter(Ressource* value) {
    return stockage.ajouter(value);
}

bool Hub::retirer_type(RessourceType type, Ressource*& value) {
    return stockage.retirer_type(type, value);
}

bool Hub::est_plein() const {
    return stockage.est_plein();
}

std::vector<RessourceType> Hub::get_all_ressource() const {
    return stockage.get_all_ressource();
}

const Stockage* Hub::get_stockage() const {
    return &stockage;
}

bool Hub::peut_entrer(Entite* e) const {
    std::lock_guard<std::mutex> lock(mtx_entite);
    return entites_presentes.size() < capacite_entree &&
           std::find(entites_presentes.begin(), entites_presentes.end(), e) == entites_presentes.end();
}

bool Hub::entrer(Entite* e) {
    std::lock_guard<std::mutex> lock(mtx_entite);
    if (!peut_entrer(e)) return false;
    entites_presentes.push_back(e);
    return true;
}

void Hub::sortir(Entite* e) {
    std::lock_guard<std::mutex> lock(mtx_entite);
    auto it = std::find(entites_presentes.begin(), entites_presentes.end(), e);
    if (it != entites_presentes.end()) entites_presentes.erase(it);
}

bool Hub::est_present(Entite* e) const {
    std::lock_guard<std::mutex> lock(mtx_entite);
    return std::find(entites_presentes.begin(), entites_presentes.end(), e) != entites_presentes.end();
}

const std::vector<Entite*>& Hub::get_liste_des_entite_presente() const {
    return entites_presentes;
}

bool Hub::dormir(Entite* e) {
    if (!est_present(e)) return false;
    if (!carte) print_error("La carte n'est pas disponible dans le hub!");
    int val = (carte->il_fait_jour() ? 5 : 10);
    e->dormir(val);
    e->add_faim(-2);
    e->add_soif(-2);
    return true;
}

std::vector<Action> Hub::get_actions_disponibles(Entite& e) {
    std::vector<Action> actions;

    if (peut_entrer(&e)) {
        actions.emplace_back("dormir", [this](Entite& ent) {
            if (this->entrer(&ent)) {
                ent.set_sleep_provider(this);
            }
        });
    }

    auto ressources_dispo = stockage.get_all_ressource();
    for (auto type : ressources_dispo) {
        std::string nom = "prendre " + RessourceInfoManager::get_info(type).nom;
        actions.emplace_back(nom, [this, type](Entite& ent) {
            Ressource* res = nullptr;
            if (this->retirer_type(type, res) && res) {
                ent.ajouter_inventaire(res);
            }
        });
    }

    auto ressources_entite = e.inventaire.get_all_ressource();
    for (auto type : ressources_entite) {
        std::string nom = "deposer " + RessourceInfoManager::get_info(type).nom;
        actions.emplace_back(nom, [this, &e, type](Entite& ent) {
            Ressource* res = nullptr;
            if (ent.retirer_type_inventaire(type, res) && res) {
                this->ajouter(res);
            }
        });
    }

    return actions;
}

nlohmann::json Hub::get_json() const {
    nlohmann::json j;
    j["stockage"] = stockage;

    {
        std::lock_guard<std::mutex> lock(mtx_entite);
        j["entites_presentes"] = nlohmann::json::array();
        for (const auto* e : entites_presentes) {
            j["entites_presentes"].push_back(e->get_name());
        }
    }

    return j;
}

void Hub::from_json(nlohmann::json json) {
    if (json.contains("stockage"))
        json.at("stockage").get_to(stockage);

    Simulation* simu = Simulation::get_instance();
    if (simu) {
        set_time_manager(simu->get_time_manager());
        set_map(simu->get_carte());
    }

    if (json.contains("entites_presentes")) {
        std::lock_guard<std::mutex> lock(mtx_entite);
        entites_presentes.clear();

        for (const auto& nom : json["entites_presentes"]) {
            std::string id = nom.get<std::string>();
            print_secondaire_attention("Entité '", id, "' sera à réassocier plus tard.");
        }
    }
}

void Hub::set_from_structure(const Structure& other) {
    const Hub* hub_other = dynamic_cast<const Hub*>(&other);
    if (!hub_other) return;

    Simulation* simu = Simulation::get_instance();
    if (simu) {
        set_time_manager(simu->get_time_manager());
        set_map(simu->get_carte());
    }

    stockage.copy_from(hub_other->get_stockage());

    for (auto& e : hub_other->get_liste_des_entite_presente()) {
        entrer(e);
    }
}

std::string Hub::get_print_string() const {
    std::string pr;
    pr += "\nStockage:" + stockage.stringifie();
    pr += "\nEntité presente: [";
    for (auto& e : entites_presentes) {
        pr += e->get_name() + ", ";
    }
    pr += "]";
    return pr;
}
