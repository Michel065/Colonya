#include "MenuScreen.h"
#include "../DisplayManager.h"

MenuScreen::MenuScreen(const sf::Font& font)
    : Screen(font, Screen_enum::Menu)
{
    float x = WINDOW_WIDTH / 2.f;
    float y = UI_SECTION_MARGIN_TOP;
    float spacing = UI_VERTICAL_SPACING;

    tools.push_back(new TextLabel("Colonya", font, {x, y - 80}, DEFAULT_FONT_SIZE_TITLE));
    tools.push_back(new Button("New", font, {x, y + spacing * 0}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
    tools.push_back(new Button("Load", font, {x, y + spacing * 1}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
    tools.push_back(new Button("Exit", font, {x, y + spacing * 2}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
}

void MenuScreen::draw(sf::RenderWindow& window) const {
    for (auto* tool : tools)
        tool->draw(window);
}

int MenuScreen::handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) {
    for (auto* tool : tools) {
        tool->lose_focus();
        if (tool->is_hovered(mouse_pos)) {
            tool->handle_click(mouse_pos);

            if (auto* btn = dynamic_cast<Button*>(tool)) {
                const std::string& label = btn->get_label();
                if (label == "New") {
                    if (manager) manager->set_screen(Screen_enum::NewMap);
                } else if (label == "Load") {
                    if (manager){
                        manager->actualise_screen_load();
                        manager->set_screen(Screen_enum::LoadMap);
                    }
                } else if (label == "Exit") {
                    if (manager) manager->close();
                }
            }
        }
    }
    return -1;
}

void MenuScreen::handle_event(const sf::Event& event) {
    // Aucun input texte ici, mais on garde la fonction au cas où
}
