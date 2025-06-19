#include "Hub.h"

Hub::Hub() : Structure(StructureType::HUB,
                StructureInfoManager::get_info(StructureType::HUB).nbr_de_base_de_coup_avant_fin_construction,
                StructureInfoManager::get_info(StructureType::HUB).state_de_base) ,stockage(taille_inv) {}

void Hub::update(Case& c) {
    // comportement spécifique, facultatif
}

bool Hub::ajouter(Ressource* value) {
    return stockage.ajouter(value);
}

bool Hub::retirer_type(RessourceType type, Ressource*& value) {
    return stockage.retirer_type(type,value);
}

bool Hub::est_plein() const {
    return stockage.est_plein();
}

std::vector<RessourceType> Hub::get_all_ressource() const {
    return stockage.get_all_ressource();
}

nlohmann::json Hub::get_json()const  {
    return {};
}

void Hub::from_json(nlohmann::json json) {

} 

void Hub::set_from_structure(const Structure& other) {
    
}

std::string Hub::get_print_string()const {
    return "";
}