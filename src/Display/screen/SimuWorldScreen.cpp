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
    if (simulation) delete simulation;
}

void SimuWorldScreen::set_simulation(Simulation* simu, DisplayManager* manager) {
    if (simulation) delete simulation;
    simulation = simu;
    if (!simulation->start()) {
        print_error("Échec du démarrage de la simulation (", simulation->get_name(), "). Retour au menu.");
        delete simulation;
        simulation = nullptr;

        if (manager) manager->set_screen(Screen_enum::Menu);
        return;
    }
    print_primaire("Simulation lancée avec succès (", simulation->get_name(), ")");
    carte=simulation->get_carte();
    map_manager=simulation->get_map_manager();

    actualiser_chunks_utilises();
}

void SimuWorldScreen::actualiser_chunks_utilises() {
    auto nouveaux_chunks = calcul_chunks_visibles();

    std::vector<std::pair<int, int>> a_deload;
    std::vector<std::pair<int, int>> a_load;

    // Étape 1 : détecter les chunks à décharger
    for (const auto& [coords, chunk] : chunks_utilises) {
        if (std::find(nouveaux_chunks.begin(), nouveaux_chunks.end(), coords) == nouveaux_chunks.end()) {
            a_deload.push_back(coords);
        }
    }

    // Étape 2 : détecter les chunks à charger
    for (const auto& coords : nouveaux_chunks) {
        if (chunks_utilises.find(coords) == chunks_utilises.end()) {
            a_load.push_back(coords);
        }
    }

    // Demander déchargement et chargement
    map_manager->demander_deload_chunk(a_deload);
    map_manager->demander_load_chunk(a_load);

    // Étape 3 : attendre et récupérer les chunks chargés
    for (const auto& coords : nouveaux_chunks) {
        if (chunks_utilises.find(coords) == chunks_utilises.end()) {
            Chunk* ch = nullptr;
            int attempts = 0;
            const int max_attempts = 20;

            while (!ch && attempts < max_attempts) {
                ch = carte->get_chunk(coords.first, coords.second);
                if (!ch) sf::sleep(sf::milliseconds(3));
                attempts++;
            }

            if (ch) {
                chunks_utilises[coords] = ch;
            } else {
                print_error("Chunk introuvable après attente (", coords.first, ",", coords.second, ")");
            }
        }
    }
}

std::pair<int, int> SimuWorldScreen::visu_to_monde(int dx, int dy) {
    int world_x = centre_case_x + dx + dy;
    int world_y = centre_case_y + dy - dx;
    return {world_x, world_y};
}

std::vector<std::pair<int, int>> SimuWorldScreen::calcul_chunks_visibles() {
    std::vector<std::pair<int, int>> visibles;

    // Calcul des coins en cases
    int x_min = centre_case_x - largeur_visible_case_demi;
    int x_max = centre_case_x + largeur_visible_case_demi;
    int y_min = centre_case_y - hauteur_visible_case_demi;
    int y_max = centre_case_y + hauteur_visible_case_demi;

    // Calcul des coins en chunks
    auto [chunk_x_min, chunk_y_min] = carte->get_chunk_coords(x_min, y_min);
    auto [chunk_x_max, chunk_y_max] = carte->get_chunk_coords(x_max, y_max);

    // Parcours des chunks visibles uniquement
    for (int cx = chunk_x_min; cx <= chunk_x_max; ++cx) {
        for (int cy = chunk_y_min; cy <= chunk_y_max; ++cy) {
            visibles.emplace_back(cx, cy);
        }
    }

    return visibles;
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
