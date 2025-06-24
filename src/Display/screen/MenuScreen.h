#ifndef _MENU_SCREEN_H
#define _MENU_SCREEN_H

#include "../../Commun/includes.h"
#include "../tool/Button.h"
#include "../tool/TextLabel.h"
#include "../../Commun/UIConfig.h"
#include "Screen.h" 

class DisplayManager;

class MenuScreen : public Screen {
public:
    MenuScreen(const sf::Font& font);
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager = nullptr) override;
    void draw(sf::RenderWindow& window) const override;
    void handle_event(const sf::Event& event) override;

private:
    std::vector<Tool*> tools;
};

#endif
