#ifndef ENTITE_H
#define ENTITE_H 

#include "../Commun/includes.h"
#include "../Tool/Action.h"
#include "../Tool/Stockage.h"

#include "Observation.h"

class Case;
class Chunk;
class MapManager;
class Map;
class TimeManager; 
class Habitation;

/*
action a ajouter faire en sorte que l'entité puisse poser des ressource sur des cases
*/


class Entite {
protected:
    std::string name;
    std::string name_texture;

    int pos_x = 0;
    int pos_y = 0;
    float orientation_vue = 0.0f; 
    float distance_vision = 5.0f;
    int portee_action=1;
    float demi_angle = 30.0f;

    int faim_max = 100;
    int soif_max = 100;
    int sommeil_max = 100;

    bool est_visible = true;

    std::atomic<int> faim;
    std::atomic<int> soif;
    std::atomic<int> sommeil;

    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> chunks_utilises;

    MapManager* map_manager = nullptr;
    Map* carte = nullptr;
    TimeManager* time_manager = nullptr;
    Habitation* sleep_provider = nullptr;

    Stockage inventaire;
    std::vector<Observation> observations;

public:
    Entite(std::string nam = "Tom", int x = 0, int y = 0);
    virtual ~Entite();

    void set_map_manager(MapManager* mm);
    void set_time_manager(TimeManager* tm);

    std::string get_name() const;

    bool is_alive();

    void set_position(int x, int y);
    std::pair<int, int> get_position() const;

    void set_orientation(float deg);
    float get_orientation() const;

    void actualiser_chunks_utilises(const std::vector<std::pair<int, int>>& coords);

    void update_sleep();
    void set_sleep_provider(Habitation* hab);

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    void run();

    std::vector<std::pair<int, int>> prendre_vision() const;
    Case* get_case_from_carte_with_coord_world(int world_x,int world_y) const;
    bool case_dans_portee(int x_case, int y_case) const;
    std::vector<Action> calculer_actions_possibles(const std::vector<std::pair<int, int>>& coords);
    std::vector<Action> action_disponible_entite();
    std::vector<Action> action_disponible_avec_ressource_inventaire();
    std::map<std::string, std::pair<int, int>> calculer_action_deplacement() const;
    
    virtual Action prise_de_decision(const std::vector<Action>& actions) = 0;

    const std::unordered_map<std::pair<int, int>, Chunk*, pair_hash>& get_chunks_utilises() const;

    // c la mais c mieux si on touche pas
    int get_faim() const;
    int get_soif() const;
    int get_sommeil() const;
    void set_faim(int val);
    void set_soif(int val);
    void set_sommeil(int val);
    void set_val_max(int faim_m, int soif_m, int sommeil_m);
    void add_faim(int delta);
    void add_soif(int delta);
    void add_sommeil(int delta);

    // ca c mieux
    bool peux_dormir() const;
    void dormir(int val);
    bool peux_manger() const;
    void manger(int val);
    bool peux_boire() const;
    void boire(int val);
    void tourner_a_gauche();
    void tourner_a_droite();
    void move_dir(int dx, int dy);

    std::vector<RessourceType> voir_toute_ressource_inventaire();
    bool ajouter_inventaire(Ressource* res);
    bool retirer_type_inventaire(RessourceType type,Ressource* res);
    
};

inline std::ostream& operator<<(std::ostream& os, const Entite& e) {
    os << "[Entite: " << e.get_name() << "]";
    return os;
}

#endif
