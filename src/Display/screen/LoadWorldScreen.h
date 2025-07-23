#ifndef LOAD_WORLD_SCREEN_H
#define LOAD_WORLD_SCREEN_H

#include "../../Commun/includes.h"
#include "Screen.h"
#include "../tool/Tool.h"
#include "../tool/Button.h"
#include "../tool/TextLabel.h"
#include "../tool/ScrollableButtonList.h"

#include "../../Simu/Simu.h"

class LoadWorldScreen : public Screen {
private:
    void delete_mondes(std::string name);
    
    std::vector<Tool*> tools;
    ScrollableButtonList liste_boutons;
    std::string monde_selectionne="";

public:
    LoadWorldScreen(const sf::Font& font);
    
    void charger_mondes();
    void draw(sf::RenderWindow& window) const override;
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) override;
    void handle_event(const sf::Event& event) override;
};

#endif
