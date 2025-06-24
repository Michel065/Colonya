#include "NewWorldScreen.h"
#include "../DisplayManager.h"
#include "../tool/InputInt.h"
#include "../tool/InputFloat.h"
#include "../tool/InputText.h" // ajout ici
#include "../tool/TextLabel.h"
#include "../tool/Button.h"
#include "../../Commun/UIConfig.h"

namespace fs = std::filesystem;

NewWorldScreen::NewWorldScreen(const sf::Font& font)
    : Screen(font, Screen_enum::NewMap)
{
    float x_center = WINDOW_WIDTH / 2.f;
    float y_start = UI_SECTION_MARGIN_TOP;
    float spacing_y = UI_VERTICAL_SPACING;
    float spacing_x = 300.f; // Espace horizontal entre les deux colonnes
    float left_x = x_center - spacing_x / 2.f;
    float right_x = x_center + spacing_x / 2.f;

    // Titre principal
    tools.push_back(new TextLabel("Creation de monde", font, {x_center, y_start - 120}, DEFAULT_FONT_SIZE_TITLE));

    // Champ centre pour le nom de fichier
    tools.push_back(new InputText("Nom du monde", font, {x_center, y_start + spacing_y * 0}, {INPUT_WIDTH, INPUT_HEIGHT}, "monde1"));

    // Sous-titre "Configuration de la carte"
    tools.push_back(new TextLabel("Configuration de la carte", font, {x_center, y_start + spacing_y * 1}, 22));

    float y_params_start = y_start + spacing_y * 2;

    // 3 lignes × 2 colonnes
    tools.push_back(new InputInt("Seed", font, {left_x, y_params_start + spacing_y * 0}, {INPUT_WIDTH, INPUT_HEIGHT}, 42));
    tools.push_back(new InputInt("Octaves", font, {right_x, y_params_start + spacing_y * 0}, {INPUT_WIDTH, INPUT_HEIGHT}, 4));
    tools.push_back(new InputFloat("Persistence", font, {left_x, y_params_start + spacing_y * 1}, {INPUT_WIDTH, INPUT_HEIGHT}, 0.5f));
    tools.push_back(new InputFloat("Scale Altitude", font, {right_x, y_params_start + spacing_y * 1}, {INPUT_WIDTH, INPUT_HEIGHT}, 0.05f));
    tools.push_back(new InputFloat("Scale Humidity", font, {left_x, y_params_start + spacing_y * 2}, {INPUT_WIDTH, INPUT_HEIGHT}, 0.05f));
    tools.push_back(new InputInt("Decalage Humidite", font, {right_x, y_params_start + spacing_y * 2}, {INPUT_WIDTH, INPUT_HEIGHT}, 1000));

    // Boutons en bas
    tools.push_back(new Button("Retour", font, {x_center - 150, y_params_start + spacing_y * 4}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
    tools.push_back(new Button("Creer", font, {x_center + 150, y_params_start + spacing_y * 4}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
}


void NewWorldScreen::draw(sf::RenderWindow& window) const {
    for (auto* tool : tools) tool->draw(window);
}

int NewWorldScreen::handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) {
    for (auto* tool : tools) {
        tool->lose_focus();
        if (tool->is_hovered(mouse_pos)) {
            tool->handle_click(mouse_pos);

            if (auto* btn = dynamic_cast<Button*>(tool)) {
                const std::string& label = btn->get_label();
                if (label == "Retour") {
                    if (manager) manager->set_screen(Screen_enum::Menu);
                    return -1;
                }

                if (label == "Creer") {
                    bool valide = true;

                    tools.erase(std::remove_if(tools.begin(), tools.end(),
                        [](Tool* t) {
                            auto* label = dynamic_cast<TextLabel*>(t);
                            return label && label->get_label() == "[Erreur] Parametre invalide";
                        }), tools.end());

                    for (Tool* t : tools) {
                        if (auto* i = dynamic_cast<InputInt*>(t)) i->set_valid(true);
                        if (auto* f = dynamic_cast<InputFloat*>(t)) f->set_valid(true);
                        if (auto* txt = dynamic_cast<InputText*>(t)) txt->set_valid(true);
                    }

                    auto check_text = [](Tool* t) -> bool {
                        auto* txt = dynamic_cast<InputText*>(t);
                        if (!txt || txt->get_value().empty()) {
                            if (txt) txt->set_valid(false);
                            return false;
                        }
                        return true;
                    };

                    auto check_int = [](Tool* t) -> bool {
                        auto* i = dynamic_cast<InputInt*>(t);
                        if (!i || i->get_value_str().empty()) {
                            if (i) i->set_valid(false);
                            return false;
                        }
                        return true;
                    };

                    auto check_float = [](Tool* t) -> bool {
                        auto* f = dynamic_cast<InputFloat*>(t);
                        if (!f || f->get_value_str().empty() || f->get_value_str() == ".") {
                            if (f) f->set_valid(false);
                            return false;
                        }
                        return true;
                    };

                    valide &= check_text(tools[1]);  // nom fichier
                    valide &= check_int(tools[2]);   // seed
                    valide &= check_int(tools[3]);   // octaves
                    valide &= check_float(tools[4]); // persistence
                    valide &= check_float(tools[5]); // scale alt
                    valide &= check_float(tools[6]); // scale hum
                    valide &= check_int(tools[7]);   // decalage

                    std::string filename = dynamic_cast<InputText*>(tools[1])->get_value();
                    std::string world_path = worlds_file + filename;
                    if (fs::exists(world_path)) {
                        dynamic_cast<InputText*>(tools[1])->set_valid(false);
                        valide = false;
                    }

                    if (!valide) {
                        tools.push_back(new TextLabel("[Erreur] Parametre invalide", font, {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 60}, 20, sf::Color::Red));
                        return -1;
                    }

                    NoiseParam param;
                    param.seed              = static_cast<unsigned int>(dynamic_cast<InputInt*>(tools[2])->get_value());
                    param.octaves           = dynamic_cast<InputInt*>(tools[3])->get_value();
                    param.persistence       = dynamic_cast<InputFloat*>(tools[4])->get_value();
                    param.scale_altitude    = dynamic_cast<InputFloat*>(tools[5])->get_value();
                    param.scale_humidity    = dynamic_cast<InputFloat*>(tools[6])->get_value();
                    param.decalage_humidity = dynamic_cast<InputInt*>(tools[7])->get_value();

                    std::cout << "[Generation] seed = " << param.seed << ", fichier = " << filename << "\n";
                    // TODO : appeler manager->generate_new_world(filename, param);
                }
            }
        }
    }
    return -1;
}

void NewWorldScreen::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        for (auto* tool : tools) {
            if (tool->has_focus()) tool->handle_text(event.text.unicode);
        }
    }
}
