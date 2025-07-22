#ifndef SIMU_WORLD_SCREEN_H
#define SIMU_WORLD_SCREEN_H

#include "../../Commun/includes.h"
#include "Screen.h"
#include "../tool/Tool.h"
#include "../tool/Button.h"
#include "../tool/Popup_Case.h"

#include "../../Simu/Simu.h"
#include "../../Map/Map.h"
#include "../tool/CalculateurDeRecouvrement.h"


class SimuWorldScreen : public Screen {
private:
    std::vector<Tool*> tools;
    Button* stop_button;
    Popup_Case* popup_case;

    Simulation* simulation = nullptr;
    MapManager* map_manager = nullptr;
    Map* carte = nullptr;

    // Position logique de la caméra (en cases)
    int centre_case_x = 25;
    int centre_case_y = 25;

    // Taille de la vue visible (moitié du nombre de cases dans chaque direction)
    int largeur_visible_case_demi = 6;
    int hauteur_visible_case_demi = 5;

    // Chunks actuellement utilisés (affichés)
    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> chunks_utilises;
    CalculateurDeRecouvrement calc;

    // Fonctions internes
    void handle_camera_movement();
    void handle_zoom(float delta);

    void actualiser_chunks_utilises();
    std::pair<int, int> visu_to_monde(float dx, float dy) const;
    std::vector<std::pair<int, int>> calcul_chunks_visibles();

    Case* get_case_from_carte_with_coord_world(int world_x,int world_y) const;
    Case* get_case_from_carte_with_coord_visuel(float dx,float dy) const;

    //pour le draw
    std::vector<sf::Texture*> liste_textures;
    mutable std::unordered_map<std::string, sf::Sprite> sprites_reutilisables;
    float pixels_par_case_x=0;
    float pixels_par_case_y=0;
    
    void init_sprites();
    void reset_simu_world();

    sf::Vector2f calcul_scale_sprite() const;
    void update_sprites_scale();
    void update_case_size();
    void update();
    sf::Vector2f conv_coord_visuel_en_coord_pixel(float case_x, float case_y) const;
    sf::Vector2f conv_coord_pixel_en_coord_realative_au_centre(sf::Vector2f pos_pixel) const;


    //draw local
    void draw_fond(sf::RenderWindow& window) const;
    void draw_case_fond(sf::RenderWindow& window,std::string name_texture,float x,float y)const;
    void draw_case_structure(sf::RenderWindow& window, Structure* s, float x, float y) const;



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
