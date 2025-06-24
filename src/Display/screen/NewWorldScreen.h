#ifndef _NEW_WORLD_H
#define _NEW_WORLD_H

#include "../../Commun/includes.h"
#include "../tool/Tool.h"
#include "Screen.h"
#include "../../Map/NoiseGenerator.h"

class DisplayManager;

class NewWorldScreen : public Screen {
public:
    NewWorldScreen(const sf::Font& font);
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager = nullptr) override;
    void draw(sf::RenderWindow& window) const override;
    void handle_event(const sf::Event& event) override;

private:
    std::vector<Tool*> tools;
};

#endif
