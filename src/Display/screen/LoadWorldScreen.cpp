#include "LoadWorldScreen.h"
#include "../DisplayManager.h"
#include "../../Commun/UIConfig.h"

LoadWorldScreen::LoadWorldScreen(const sf::Font& font)
    : Screen(font, Screen_enum::LoadMap),
      liste_boutons(font)
{
    float x = WINDOW_WIDTH / 2.f;
    float y = UI_SECTION_MARGIN_TOP;

    tools.push_back(new TextLabel("Charger un monde existant", font, {x, y - 80}, DEFAULT_FONT_SIZE_TITLE));
    tools.push_back(new Button("Retour", font, {x - 150, WINDOW_HEIGHT - 80}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
    tools.push_back(new Button("Load", font, {x + 150, WINDOW_HEIGHT - 80}, {BUTTON_WIDTH, BUTTON_HEIGHT}));

    // Navigation gauche/droite
    tools.push_back(new Button("<", font, {100, WINDOW_HEIGHT / 2.f}, {40, 40}));
    tools.push_back(new Button(">", font, {WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2.f}, {40, 40}));

    // Charger les mondes et peupler la liste
    charger_mondes();
}

void LoadWorldScreen::charger_mondes() {
    liste_boutons.clear();
    liste_boutons.reset_index();
    for (const auto& entry : fs::directory_iterator(worlds_file)) {
        if (entry.is_directory()) {
            liste_boutons.add_label(entry.path().filename().string());
        }
    }
}

void LoadWorldScreen::draw(sf::RenderWindow& window) const {
    for (auto* tool : tools) tool->draw(window);
    liste_boutons.draw(window);
}

int LoadWorldScreen::handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) {
    liste_boutons.handle_click(mouse_pos);

    std::string selection = liste_boutons.get_selected_label();
    if (!selection.empty()) {
        monde_selectionne = selection;
        std::cout << "[Sélection] Monde = " << monde_selectionne << "\n";
    }
    
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
                    if (monde_selectionne.empty()) {
                        print_error("Aucun monde sélectionné.");
                        return -1;
                    }
                    print_primaire("Chargement du monde sélectionné (", monde_selectionne, ")");
                    Simulation* simu=new Simulation(monde_selectionne);
                    if (manager) manager->set_simu_in_simu_screen(simu);
                    if (manager) manager->set_screen(Screen_enum::Simu);
                }
            }
        }
    }
    return -1;
}

void LoadWorldScreen::handle_event(const sf::Event& event) {
    // Rien à gérer pour l’instant
}
