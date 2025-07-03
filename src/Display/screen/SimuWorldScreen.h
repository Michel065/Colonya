#ifndef SIMU_WORLD_SCREEN_H
#define SIMU_WORLD_SCREEN_H

#include "../../Commun/includes.h"
#include "Screen.h"
#include "../tool/Tool.h"
#include "../tool/Button.h"

#include "../../Simu/Simu.h"
#include "../../Map/Map.h"

class SimuWorldScreen : public Screen {
private:
    // Vue SFML utilisée uniquement pour afficher (ne déplace plus la caméra)
    sf::View vue;
    float zoom_level = 1.f;

    std::vector<Tool*> tools;
    Button* stop_button;

    // Simulation et carte
    Simulation* simulation = nullptr;
    MapManager* map_manager = nullptr;
    Map* carte = nullptr;

    // Position logique de la caméra (en cases)
    int centre_case_x = 9;
    int centre_case_y = 6;

    // Taille de la vue visible (moitié du nombre de cases dans chaque direction)
    int largeur_visible_case_demi = 5;
    int hauteur_visible_case_demi = 5;

    // Chunks actuellement utilisés (affichés)
    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> chunks_utilises;

    // Fonctions internes
    void handle_camera_movement(); // plus besoin de delta
    void handle_zoom(float delta);

    void actualiser_chunks_utilises();
    std::pair<int, int> visu_to_monde(int dx, int dy);
    std::vector<std::pair<int, int>> calcul_chunks_visibles();

public:
    SimuWorldScreen(const sf::Font& font);
    ~SimuWorldScreen();

    void set_simulation(Simulation* simu, DisplayManager* manager);

    void draw(sf::RenderWindow& window) const override;
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) override;
    void handle_event(const sf::Event& event) override;

    void print_chunk_charge();
};

#endif
