#ifndef SIMU_WORLD_SCREEN_H
#define SIMU_WORLD_SCREEN_H

#include "../../Commun/includes.h"
#include "Screen.h"
#include "../tool/Tool.h"
#include "../tool/Button.h"

class SimuWorldScreen : public Screen {
private:
    sf::View vue;
    float zoom_level = 1.f;
    sf::Vector2f camera_position;

    std::vector<Tool*> tools;
    Button* stop_button;

    void handle_camera_movement(float delta);
    void handle_zoom(float delta);

public:
    SimuWorldScreen(const sf::Font& font);
    ~SimuWorldScreen();

    void draw(sf::RenderWindow& window) const override;
    int handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) override;
    void handle_event(const sf::Event& event) override;
};

#endif
