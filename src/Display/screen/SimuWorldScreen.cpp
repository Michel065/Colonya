#include "SimuWorldScreen.h"
#include "../tool/Button.h"
#include "../../Commun/UIConfig.h"
#include "../DisplayManager.h"

SimuWorldScreen::SimuWorldScreen(const sf::Font& font)
    : Screen(font, Screen_enum::Simu)
{
    vue.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    vue.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    camera_position = vue.getCenter();

    stop_button = new Button("Stop", font, {WINDOW_WIDTH - 100.f, 30.f}, {100.f, 40.f});
    tools.push_back(stop_button);
}

SimuWorldScreen::~SimuWorldScreen() {
    for (Tool* t : tools) delete t;
}

void SimuWorldScreen::draw(sf::RenderWindow& window) const {
    sf::View old_view = window.getView();
    window.setView(vue);

    // TODO : dessiner la carte ici

    window.setView(old_view);

    for (auto* tool : tools) tool->draw(window);
}

int SimuWorldScreen::handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) {
    for (Tool* t : tools) {
        if (t->is_hovered(mouse_pos)) {
            t->handle_click(mouse_pos);
            if (t == stop_button && manager) {
                manager->set_screen(Screen_enum::Menu);
            }
        }
    }
    return -1;
}

void SimuWorldScreen::handle_camera_movement(float delta) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) camera_position.x -= delta;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) camera_position.x += delta;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) camera_position.y -= delta;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) camera_position.y += delta;
    vue.setCenter(camera_position);
}

void SimuWorldScreen::handle_zoom(float delta) {
    zoom_level += delta;
    zoom_level = std::clamp(zoom_level, 0.5f, 2.0f);
    vue.setSize(WINDOW_WIDTH * zoom_level, WINDOW_HEIGHT * zoom_level);
}

void SimuWorldScreen::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        handle_camera_movement(30.f);
        if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
            handle_zoom(-0.1f);
        if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash)
            handle_zoom(0.1f);
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        handle_zoom(event.mouseWheelScroll.delta > 0 ? -0.1f : 0.1f);
    }
}
