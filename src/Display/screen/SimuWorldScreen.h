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
    sf::View vue;
    float zoom_level = 1.f;// soit 10case de large
    sf::Vector2f camera_position;

    std::vector<Tool*> tools;
    Button* stop_button;

    //la simualtion
    Simulation* simulation=nullptr;
    MapManager* map_manager=nullptr;

    Map*carte=nullptr;

    void handle_camera_movement(float delta);
    void handle_zoom(float delta);

    int centre_case_x = 0;
    int centre_case_y = 0;
    int largeur_visible_case_demi = 5; // en cases
    int hauteur_visible_case_demi = 5;
    
    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> chunks_utilises;

    void actualiser_chunks_utilises();
    std::pair<float, float> visu_to_monde(int dx, int dy);
    std::vector<std::pair<int, int>> calcul_chunks_visibles();

public:
    SimuWorldScreen(const sf::Font& font);
    ~SimuWorldScreen();

    void set_simulation(Simulation* simu, DisplayManager* manager);

    void draw(sf::RenderWindow& window) const override;
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) override;
    void handle_event(const sf::Event& event) override;
};

#endif
