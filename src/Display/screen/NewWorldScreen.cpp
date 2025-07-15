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
    float spacing_x = 300.f;
    float left_x = x_center - spacing_x / 2.f;
    float right_x = x_center + spacing_x / 2.f;

    // TITRE
    tools.push_back(new TextLabel("Creation de monde", font, {x_center, y_start - 120}, DEFAULT_FONT_SIZE_TITLE));

    // NOM DU MONDE
    nom = new InputText("Nom du monde", font, {x_center, y_start + spacing_y * 0}, {INPUT_WIDTH, INPUT_HEIGHT}, "001");
    tools.push_back(nom);

    tools.push_back(new TextLabel("Configuration de la carte", font, {x_center, y_start + spacing_y * 1}, 22));

    float y_params_start = y_start + spacing_y * 2;

    // PARAMS CARTE
    seed       = new InputInt("Seed", font, {left_x, y_params_start + spacing_y * 0}, {INPUT_WIDTH, INPUT_HEIGHT}, 42);
    octaves    = new InputInt("Octaves", font, {right_x, y_params_start + spacing_y * 0}, {INPUT_WIDTH, INPUT_HEIGHT}, 4);
    pers       = new InputFloat("Persistence", font, {left_x, y_params_start + spacing_y * 1}, {INPUT_WIDTH, INPUT_HEIGHT}, 0.5f);
    scale_alt  = new InputFloat("Scale Altitude", font, {right_x, y_params_start + spacing_y * 1}, {INPUT_WIDTH, INPUT_HEIGHT}, 0.05f);
    scale_hum  = new InputFloat("Scale Humidity", font, {left_x, y_params_start + spacing_y * 2}, {INPUT_WIDTH, INPUT_HEIGHT}, 0.05f);
    decal      = new InputInt("Decalage Humidite", font, {right_x, y_params_start + spacing_y * 2}, {INPUT_WIDTH, INPUT_HEIGHT}, 1000);

    tools.push_back(seed);
    tools.push_back(octaves);
    tools.push_back(pers);
    tools.push_back(scale_alt);
    tools.push_back(scale_hum);
    tools.push_back(decal);

    // BOUTONS
    tools.push_back(new Button("Retour", font, {WINDOW_WIDTH / 2.f - 150, WINDOW_HEIGHT - 80}, {BUTTON_WIDTH, BUTTON_HEIGHT}));
    tools.push_back(new Button("Creer", font, {WINDOW_WIDTH / 2.f + 150, WINDOW_HEIGHT - 80}, {BUTTON_WIDTH, BUTTON_HEIGHT}));

    // Label d'erreur initialisé (vide, non affiché)
    label_erreur = new TextLabel("", font, {left_x, y_params_start + spacing_y * 3.5f}, 20, sf::Color::Red);
    tools.push_back(label_erreur);
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
                    if (label_erreur) label_erreur->set_text("");

                    // Reset couleurs
                    nom->set_valid(true);
                    seed->set_valid(true);
                    octaves->set_valid(true);
                    pers->set_valid(true);
                    scale_alt->set_valid(true);
                    scale_hum->set_valid(true);
                    decal->set_valid(true);

                    // Vérif nom monde
                    if (!nom->is_valid()) {
                        label_erreur->set_text("[Erreur] Nom de monde invalide");
                        return -1;
                    }

                    
                    std::string filename = nom->get_value();
                    std::string world_path = worlds_file + filename;
                    if (fs::exists(world_path)) {
                        nom->set_valid(false);
                        label_erreur->set_text("[Erreur] Ce monde existe deja");
                        return -1;
                    }

                    // Vérif numériques
                    if (!seed->is_valid()) {
                        seed->set_valid(false);
                        label_erreur->set_text("[Erreur] Seed invalide");
                        return -1;
                    }
                    if (!octaves->is_valid()) {
                        octaves->set_valid(false);
                        label_erreur->set_text("[Erreur] Octaves invalide");
                        return -1;
                    }
                    if (!pers->is_valid()) {
                        pers->set_valid(false);
                        label_erreur->set_text("[Erreur] Persistence invalide");
                        return -1;
                    }
                    if (!scale_alt->is_valid()) {
                        scale_alt->set_valid(false);
                        label_erreur->set_text("[Erreur] Échelle altitude invalide");
                        return -1;
                    }
                    if (!scale_hum->is_valid()) {
                        scale_hum->set_valid(false);
                        label_erreur->set_text("[Erreur] Échelle humidité invalide");
                        return -1;
                    }
                    if (!decal->is_valid()) {
                        decal->set_valid(false);
                        label_erreur->set_text("[Erreur] Décalage humidité invalide");
                        return -1;
                    }

                    // Tous les paramètres sont OK
                    print_status(true, "Paramètres valides, génération du monde");
                    if (manager) manager->draw_loading_screen();

                    NoiseParam* param = new NoiseParam;
                    param->seed              = static_cast<unsigned int>(seed->get_value());
                    param->octaves           = octaves->get_value();
                    param->persistence       = pers->get_value();
                    param->scale_altitude    = scale_alt->get_value();
                    param->scale_humidity    = scale_hum->get_value();
                    param->decalage_humidity = decal->get_value();

                    print("[Generation] seed = ", param->seed, ", fichier = ", filename);
                    
                    // Création unique de la simulation
                    Simulation::create(filename,param);
                    Simulation* simu = Simulation::get_instance();
                    
                    manager->set_simu_in_simu_screen(simu);
                    print_status(false, "Paramètres valides, génération du monde");
                    manager->set_screen(Screen_enum::Simu);
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
