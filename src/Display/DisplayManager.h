#ifndef _DISPLAY_MANAGER_H
#define _DISPLAY_MANAGER_H

#include "../Commun/includes.h"
#include "../Commun/UIConfig.h"
#include "./screen/MenuScreen.h"
#include "./screen/NewWorldScreen.h"
#include "./screen/LoadWorldScreen.h"
#include "./screen/SimuWorldScreen.h"

#include "../Simu/Simu.h"

class DisplayManager {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Font font;

    std::vector<Screen*> screens;
    Screen_enum current_screen = Screen_enum::LoadMap;

    SimuWorldScreen* screen_de_la_simu=nullptr;

    void handle_events();
    void update(float dt);
    void render();

public:
    DisplayManager();
    ~DisplayManager();

    void run();

    void close();
    void set_screen(Screen_enum new_screen);

    void set_simu_in_simu_screen(Simulation*simu);
};

#endif
