#include "DisplayManager.h"

DisplayManager::DisplayManager() 
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE)
{
    window.setFramerateLimit(60);
    font.loadFromFile(police_source);

    screens.push_back(new MenuScreen(font));
    screens.push_back(new NewWorldScreen(font));
    screens.push_back(new LoadWorldScreen(font));
    screen_de_la_simu=new SimuWorldScreen(font);
    screens.push_back(screen_de_la_simu);
}

DisplayManager::~DisplayManager() {
    for (auto* screen : screens) delete screen;
}

void DisplayManager::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handle_events();
        update(dt);
        render();
    }
}

void DisplayManager::handle_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
            for (auto* screen : screens) {
                if (screen->is_active(current_screen)) {
                    screen->handle_click(mouse_pos, this);
                }
            }
        }

        if (event.type == sf::Event::TextEntered) {
            for (auto* screen : screens) {
                if (screen->is_active(current_screen)) {
                    screen->handle_event(event); // pour gérer l’input texte
                }
            }
        }
    }
}

void DisplayManager::update(float dt) {
    // Logique par screen possible plus tard
}

void DisplayManager::render() {
    window.clear(sf::Color(240, 230, 220));  // Couleur douce

    for (auto* screen : screens) {
        if (screen->is_active(current_screen)) {
            screen->draw(window);
        }
    }

    window.display();
}

void DisplayManager::close() {
    window.close();
}

void DisplayManager::set_screen(Screen_enum new_screen) {
    current_screen = new_screen;
}

void DisplayManager::set_simu_in_simu_screen(Simulation*simu){
    screen_de_la_simu->set_simulation(simu,this);
}

