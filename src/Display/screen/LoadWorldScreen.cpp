#include "LoadWorldScreen.h"
#include "../DisplayManager.h"
#include "../tool/Button.h"
#include "../tool/TextLabel.h"
#include "../../Commun/UIConfig.h"


LoadWorldScreen::LoadWorldScreen(const sf::Font& font)
    : Screen(font, Screen_enum::LoadMap)
{
    float x = WINDOW_WIDTH / 2.f;
    float y = UI_SECTION_MARGIN_TOP;

    tools.push_back(new TextLabel("Charger un monde existant", font, {x, y - 80}, DEFAULT_FONT_SIZE_TITLE));

    // Boutons de navigation
    tools.push_back(new Button("<", font, {100, WINDOW_HEIGHT / 2.f}, {40, 40}));
    tools.push_back(new Button(">", font, {WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2.f}, {40, 40}));

    // Boutons fixes en bas
    tools.push_back(new Button("Retour", font, {x - 150, WINDOW_HEIGHT - 80}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
    tools.push_back(new Button("Load", font, {x + 150, WINDOW_HEIGHT - 80}, {BUTTON_WIDTH, BUTTON_HEIGHT}));

    charger_mondes();
    update_affichage_mondes();
}

void LoadWorldScreen::charger_mondes() {
    mondes.clear();
    for (const auto& entry : fs::directory_iterator(worlds_file)) {
        if (entry.is_directory()) {
            mondes.push_back(entry.path().filename().string());
        }
    }
}

void LoadWorldScreen::update_affichage_mondes() {
    // Supprimer anciens boutons liés aux mondes
    tools.erase(std::remove_if(tools.begin(), tools.end(), [this](Tool* t) {
        Button* b = dynamic_cast<Button*>(t);
        if (!b) return false;
        std::string label = b->get_label();
        return std::find(mondes.begin(), mondes.end(), label) != mondes.end();
    }), tools.end());

    const int nb_par_page = 6;
    float start_y = WINDOW_HEIGHT / 2.f - (nb_par_page / 2.f) * (INPUT_HEIGHT + 10.f);
    int debut = page_index * nb_par_page;

    for (int i = 0; i < nb_par_page && (debut + i) < static_cast<int>(mondes.size()); ++i) {
        std::string nom = mondes[debut + i];

        // Création d'un texte temporaire pour ajuster la taille
        sf::Text text_temp(nom, font, DEFAULT_FONT_SIZE_LABEL);
        sf::FloatRect bounds = text_temp.getLocalBounds();
        float largeur = bounds.width * 1.2 + 20.f;
        float hauteur = bounds.height + 20.f;

        sf::Vector2f position = {
            WINDOW_WIDTH / 2.f,  // Centrage automatique dans Button
            start_y + i * (hauteur + 10.f)
        };

        Button* b = new Button(nom, font, position, {largeur, hauteur});

        if (nom == monde_selectionne) {
            b->set_color(sf::Color(255, 165, 0)); // orange
            selected_button = b;
        } else {
            b->set_color(sf::Color::White);
        }

        tools.push_back(b);
    }
}



void LoadWorldScreen::draw(sf::RenderWindow& window) const {
    for (auto* tool : tools) tool->draw(window);
}

int LoadWorldScreen::handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) {
    for (auto* tool : tools) {
        tool->lose_focus();
        if (tool->is_hovered(mouse_pos)) {
            tool->handle_click(mouse_pos);
            if (auto* btn = dynamic_cast<Button*>(tool)) {
                std::string label = btn->get_label();

                if (label == "Retour") {
                    if (manager) manager->set_screen(Screen_enum::Menu);
                    return -1;
                }
                if (label == "Load") {
                    std::cout << "[TODO] Charger le monde selectionne\n";
                }
                if (label == "<") {
                    if (page_index > 0) {
                        page_index--;
                        update_affichage_mondes();
                    }
                }
                if (label == ">") {
                    int nb_pages = (mondes.size() + 5) / 6;
                    if (page_index < nb_pages - 1) {
                        page_index++;
                        update_affichage_mondes();
                    }
                }
                if (!label.empty() && std::find(mondes.begin(), mondes.end(), label) != mondes.end()) {
                    monde_selectionne = label;

                    if (selected_button)
                        selected_button->set_color(sf::Color::White); // Réinitialise l'ancien bouton
                    btn->set_color(sf::Color(255, 165, 0)); // Orange
                    selected_button = btn;

                    std::cout << "[Selection] Monde = " << monde_selectionne << "\n";
                }
            }
        }
    }
    return -1;
}

void LoadWorldScreen::handle_event(const sf::Event& event) {
    // Rien pour l'instant
}
