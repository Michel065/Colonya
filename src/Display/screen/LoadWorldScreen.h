#ifndef LOAD_WORLD_SCREEN_H
#define LOAD_WORLD_SCREEN_H

#include "../../Commun/includes.h"
#include "Screen.h"
#include "../tool/Tool.h"
#include "../tool/Button.h"

class LoadWorldScreen : public Screen {
private:
    void charger_mondes();
    void update_affichage_mondes();

    std::vector<std::string> mondes;
    std::vector<Tool*> tools;
    int page_index = 0;
    std::string monde_selectionne;
    Button* selected_button = nullptr;
    
public:
    LoadWorldScreen(const sf::Font& font);
    void draw(sf::RenderWindow& window) const override;
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) override;
    void handle_event(const sf::Event& event) override;
};

#endif