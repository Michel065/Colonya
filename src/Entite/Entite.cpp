#include "Entite.h"
#include "../Map/Map.h"
#include "../Map/Chunk.h"
#include "../Map/Case.h"
#include "../Synchronisation/TimeManager.h"
#include "../Structure/types/Habitation.h"

Entite::Entite(std::string nam, int x, int y)
    : name(nam), pos_x(x), pos_y(y),
      faim(faim_max), soif(soif_max), sommeil(sommeil_max) {}

Entite::~Entite() {}

void Entite::set_map(Map* m) {map = m;}
void Entite::set_time_manager(TimeManager* tm) {time_manager = tm;}

std::string Entite::get_name() const {return name;}

bool Entite::is_alive(){return faim>0 && soif>0 && sommeil>0;}

const std::unordered_map<std::pair<int, int>, Chunk*, pair_hash>& Entite::get_chunks_utilises() const {
    return chunks_utilises;
}

int Entite::get_faim() const { return faim.load(); }
int Entite::get_soif() const { return soif.load(); }
int Entite::get_sommeil() const { return sommeil.load(); }

void Entite::set_faim(int val) { faim = val; }
void Entite::set_soif(int val) { soif = val; }
void Entite::set_sommeil(int val) { sommeil = val; }

void Entite::set_val_max(int faim_m, int soif_m, int sommeil_m) {
    faim_max = faim_m;
    soif_max = soif_m;
    sommeil_max = sommeil_m;
}

void Entite::add_faim(int delta) {
    faim.store(std::clamp(faim.load() + delta, 0, faim_max));
}

void Entite::add_soif(int delta) {
    soif.store(std::clamp(soif.load() + delta, 0, soif_max));
}

void Entite::add_sommeil(int delta) {
    sommeil.store(std::clamp(sommeil.load() + delta, 0, sommeil_max));
}


bool Entite::peux_dormir() const {return sommeil < sommeil_max;}
void Entite::dormir(int val) {sommeil = std::min(sommeil.load() + val, sommeil_max);}

bool Entite::peux_manger() const {return faim < faim_max;}
void Entite::manger(int val) {faim = std::min(faim.load() + val, faim_max);}

bool Entite::peux_boire() const {return soif < soif_max;}
void Entite::boire(int val) {soif = std::min(soif.load() + val, soif_max);}

void Entite::set_position(int x, int y) {
    pos_x = x;
    pos_y = y;
}

std::pair<int, int> Entite::get_position() const {return { pos_x, pos_y };}
void Entite::set_orientation(float deg) {orientation_vue = deg;}
float Entite::get_orientation() const {    return orientation_vue;}

void Entite::actualiser_chunks_utilises(const std::vector<std::pair<int, int>>& coords) {
    std::unordered_set<std::pair<int, int>, pair_hash> nouveaux_chunks;
    for (const auto& [x, y] : coords) {
        int chunk_x = x / CHUNK_SIZE;
        int chunk_y = y / CHUNK_SIZE;
        nouveaux_chunks.emplace(chunk_x, chunk_y);
    }

    for (const auto& chunk_coord : nouveaux_chunks) {
        if (!chunks_utilises.count(chunk_coord)) {
            Chunk* ch = map->demander_load_chunk(chunk_coord.first, chunk_coord.second, true);
            if (ch) {
                chunks_utilises[chunk_coord] = ch;
            }
        }
    }

    for (auto it = chunks_utilises.begin(); it != chunks_utilises.end(); ) {
        if (!nouveaux_chunks.count(it->first)) {
            map->demander_load_chunk(it->first.first, it->first.second, false);
            it = chunks_utilises.erase(it);
        } else {
            ++it;
        }
    }
}

void Entite::run() {
    while (time_manager && time_manager->running() && is_alive()) {
        time_manager->waitNextTick();

        if (sleep_provider) {
            update_sleep();
            continue;  // Court-circuite la boucle, pas de décision classique
        }

        // Logique normale
        auto coords = prendre_vision();
        actualiser_chunks_utilises(coords);
        auto actions = calculer_actions_possibles(coords);
        Action decision = prise_de_decision(actions);
        decision.executer(*this);
    }
}

std::vector<std::pair<int, int>> Entite::prendre_vision() const {
    std::vector<std::pair<int, int>> resultats;

    const float orientation_rad = orientation_vue * (M_PI / 180.0f);  // Conversion en radians

    for (int dx = -distance_vision; dx <= distance_vision; ++dx) {
        for (int dy = -distance_vision; dy <= distance_vision; ++dy) {
            int tx = pos_x + dx;
            int ty = pos_y + dy;

            if (dx == 0 && dy == 0) continue;  // On ignore la case où se trouve l'entité

            // Calcul du vecteur relatif
            float vect_x = static_cast<float>(dx);
            float vect_y = static_cast<float>(dy);
            float distance = std::sqrt(vect_x * vect_x + vect_y * vect_y);
            if (distance > distance_vision) continue;

            // Calcul angle entre orientation et vecteur (dx, dy)
            float angle = std::atan2(vect_y, vect_x) * (180.0f / M_PI);  // en degrés
            float relative_angle = angle - orientation_vue;

            // Normalisation de l'angle [-180°, 180°]
            while (relative_angle < -180.0f) relative_angle += 360.0f;
            while (relative_angle > 180.0f)  relative_angle -= 360.0f;

            if (std::abs(relative_angle) <= demi_angle) {
                resultats.emplace_back(tx, ty);
            }
        }
    }

    return resultats;
}

Case* Entite::get_case_from_carte_with_coord_world(int world_x,int world_y) const {
    auto chunk_name = carte->get_chunk_coords(world_x, world_y);
    auto [case_x, case_y] = carte->get_local_coords(world_x, world_y);

    auto it = chunks_utilises.find(chunk_name);
    if (it == chunks_utilises.end()) {
        print_error("Chunk non chargé pour la case (", world_x, ",", world_y, ")");
        return nullptr;
    }

    return it->second->at(case_x, case_y);
}

std::vector<Action> Entite::calculer_actions_possibles(const std::vector<std::pair<int, int>>& coords) const {
    std::vector<Action> actions;

    for (const auto& [x, y] : coords) {
        Case* c = get_case_from_carte_with_coord_world(x, y);
        if (c) {
            auto actions_case = c->get_actions_disponibles(*this);
            actions.insert(actions.end(), actions_case.begin(), actions_case.end());
        }
    }

    auto propres = action_disponible_entite();
    actions.insert(actions.end(), propres.begin(), propres.end());

    return actions;
}

void Entite::tourner_a_gauche() {
    orientation_vue -= 15.0f;
    if (orientation_vue < 0.0f) orientation_vue += 360.0f;
}

void Entite::tourner_a_droite() {
    orientation_vue += 15.0f;
    if (orientation_vue >= 360.0f) orientation_vue -= 360.0f;
}


struct DirInfo {
    int dx, dy;
    float angle_ref;
};

static const std::vector<DirInfo> directions = {
    { 0, -1, 270.0f},  // Nord
    { 1,  0,   0.0f},  // Est
    { 0,  1,  90.0f},  // Sud
    {-1,  0, 180.0f}   // Ouest
};

static const std::vector<std::pair<std::string, float>> roles = { 
    {"avancer", 0.0f}, 
    {"droite", 90.0f}, 
    {"reculer", 180.0f}, 
    {"gauche", -90.0f} 
};

std::map<std::string, std::pair<int, int>> Entite::calculer_action_deplacement() const {
    std::map<std::string, std::pair<int, int>> dir_map;

    for (const auto& [role, offset] : roles) {
        float target_angle = orientation_vue + offset;
        if (target_angle >= 360.0f) target_angle -= 360.0f;
        if (target_angle < 0.0f) target_angle += 360.0f;

        float best_diff = 360.0f;
        const DirInfo* best_dir = nullptr;

        for (const auto& dir : directions) {
            float diff = std::abs(target_angle - dir.angle_ref);
            if (diff > 180.0f) diff = 360.0f - diff;

            if (diff < best_diff) {
                best_diff = diff;
                best_dir = &dir;
            }
        }

        if (best_dir) {
            dir_map[role] = {best_dir->dx, best_dir->dy};
        }
    }

    return dir_map;
}

std::vector<Action> Entite::action_disponible_avec_ressource_inventaire() const {
    return inventaire.get_all_action_ressource(const_cast<Entite&>(*this));
}


std::vector<Action> Entite::action_disponible_entite() const {
    std::vector<Action> actions;

    actions.emplace_back("tourner_a_gauche", [this](Entite& e) {
        e.tourner_a_gauche();
    });

    actions.emplace_back("tourner_a_droite", [this](Entite& e) {
        e.tourner_a_droite();
    });

    auto dir_map = calculer_action_deplacement();

    for (const auto& [nom, delta] : dir_map) {
        int nx = pos_x + delta.first;
        int ny = pos_y + delta.second;
        Case* c = get_case_from_carte_with_coord_world(x, y);
        if (c && c->is_walkable()) {
            actions.emplace_back(nom, [this, dx=delta.first, dy=delta.second](Entite& e) {
                e.move_dir(dx, dy);
            });
        }
    }

    // Ajout des actions disponibles liées à l'inventaire
    auto actions_inv = action_disponible_avec_ressource_inventaire();
    actions.insert(actions.end(), actions_inv.begin(), actions_inv.end());

    return actions;
}

void Entite::move_dir(int dx, int dy) {
    int nx = pos_x + dx;
    int ny = pos_y + dy;
    set_position(nx, ny);
}

void Entite::update_sleep() {
    if (!sleep_provider) return;

    // Appel direct au comportement dormir générique
    sleep_provider->dormir(this);

    if (!this->peux_dormir()) {
        sleep_provider->sortir(this);
        sleep_provider = nullptr;
    }
}

void Entite::set_sleep_provider(Habitation* hab){
    sleep_provider=hab;
}

bool Entite::ajouter_inventaire(Ressource* res){
    inventaire.ajouter(res)
}

bool Entite::retirer_type_inventaire(RessourceType type,Ressource* res){
    return inventaire.retirer_type(type,res);
}
