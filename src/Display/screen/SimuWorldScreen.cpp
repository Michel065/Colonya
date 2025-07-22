#include "SimuWorldScreen.h"
#include "../tool/Button.h"
#include "../../Commun/UIConfig.h"
#include "../DisplayManager.h"

SimuWorldScreen::SimuWorldScreen(const sf::Font& font)
    : Screen(font, Screen_enum::Simu)
{
    stop_button = new Button("Stop", font, {WINDOW_WIDTH - 100.f, 30.f}, {100.f, 40.f});
    tools.push_back(stop_button);
    popup_case = new Popup_Case(font);
    tools.push_back(popup_case);

    init_sprites();
}

SimuWorldScreen::~SimuWorldScreen() {
    for (Tool* t : tools) delete t;
    if (simulation) Simulation::destroy();
    for (auto& tx : liste_textures) delete tx;
}

void SimuWorldScreen::set_simulation(Simulation* simu, DisplayManager* manager) {
    if (!simulation) {
        print_error("simulation non transmise a simu world screen");
        delete simulation;
    }
    simulation = simu;
    if (!simulation->start()) {
        print_error("Échec du démarrage de la simulation (", simulation->get_name(), "). Retour au menu.");
        simulation->stop();
        Simulation::destroy();
        simulation = nullptr;
        if (manager) manager->set_screen(Screen_enum::Menu);
        return;
    }
    print_primaire("Simulation lancée avec succès (", simulation->get_name(), ")");
    carte=simulation->get_carte();
    map_manager=simulation->get_map_manager();
    auto coord = carte->get_coord_spawn();
    centre_case_x = coord.first;
    centre_case_y = coord.second;
    print_primaire("coord: set simulation ",centre_case_x,"x",centre_case_y);

    update();
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
    
    // ⚠️ Étape 1.5 : suppression réelle des chunks inutiles
    for (const auto& coords : a_deload) {
        chunks_utilises.erase(coords);
    }

    // Étape 2 : détecter les chunks à charger
    for (const auto& coords : nouveaux_chunks) {
        if (chunks_utilises.find(coords) == chunks_utilises.end()) {
            a_load.push_back(coords);
        }
    }

    // Demander déchargement et chargement
    map_manager->demander_deload_chunk(a_deload, true); // true = sort du chunk
    std::vector<Chunk*> chunks_charges = map_manager->demander_load_chunk(a_load, true); // true = on entre dans le chunk

    // Étape 3 : stocker les chunks réellement chargés
    for (std::size_t i = 0; i < a_load.size(); ++i) {
        if (chunks_charges[i]) {
            chunks_utilises[a_load[i]] = chunks_charges[i];
        } else {
            print_error("Chunk introuvable après chargement (", a_load[i].first, ",", a_load[i].second, ")");
        }
    }
}

void SimuWorldScreen::print_chunk_charge(){
    std::string msg = "chunk chargé: {";
    for (const auto& coords : chunks_utilises) {
        msg += "(" + std::to_string(coords.first.first) + "x" + std::to_string(coords.first.second) + "),";
    }
    msg += "}";
    print_secondaire(msg);
}

std::pair<int, int> SimuWorldScreen::visu_to_monde(float dx, float dy) const{
    int world_x = centre_case_x + dx + dy;
    int world_y = centre_case_y + dy - dx;
    return {world_x, world_y};
}

std::vector<std::pair<int, int>> SimuWorldScreen::calcul_chunks_visibles() {
    std::vector<std::pair<int, int>> visibles;
    int marge = 2;

    // Points en visu -> monde
    std::vector<std::pair<float, float>> coins_monde = {
        visu_to_monde(-largeur_visible_case_demi - marge,  hauteur_visible_case_demi + marge), // haut gauche
        visu_to_monde(-largeur_visible_case_demi - marge, -hauteur_visible_case_demi - marge), // bas gauche
        visu_to_monde( largeur_visible_case_demi + marge, -hauteur_visible_case_demi - marge), // bas droite
        visu_to_monde( largeur_visible_case_demi + marge,  hauteur_visible_case_demi + marge)  // haut droite
    };

    std::vector<Point> points;

    for (const auto& [wx, wy] : coins_monde) {
        auto [chunk_x, chunk_y] = carte->get_chunk_coords(wx, wy);
        auto [offset_x, offset_y] = carte->get_local_coords_float(wx, wy);
        points.push_back(Point{
            static_cast<float>(chunk_x) + offset_x,
            static_cast<float>(chunk_y) + offset_y
        });
    }

    visibles = calc.get_chunk(points);
    
    return visibles;
}


void SimuWorldScreen::draw_fond(sf::RenderWindow& window) const {
    float x,y;
    for (int iy = -hauteur_visible_case_demi-1; iy <= hauteur_visible_case_demi; ++iy) {
        for (float decalage = 0; decalage <=0.5 ; decalage+=0.5) {
            for (int ix = -largeur_visible_case_demi-1; ix <= largeur_visible_case_demi; ++ix) {
                x=ix+decalage;
                y=iy+decalage;
                Case* c = get_case_from_carte_with_coord_visuel(x, y);
                if (!c) continue;
                draw_case_fond(window, c->get_terrain()->name, x, y);
                if (c->get_structure()) {
                    draw_case_structure(window, c->get_structure(), x, y);
                }

            }
        }
    }

}

void SimuWorldScreen::draw(sf::RenderWindow& window) const {
    draw_fond(window);
    for (auto* tool : tools) tool->draw(window);
}

Case* SimuWorldScreen::get_case_from_carte_with_coord_world(int world_x,int world_y) const {
    auto chunk_name = carte->get_chunk_coords(world_x, world_y);
    auto [case_x, case_y] = carte->get_local_coords(world_x, world_y);

    auto it = chunks_utilises.find(chunk_name);
    if (it == chunks_utilises.end()) {
        print_error("Chunk non chargé pour la case (", world_x, ",", world_y, ")");
        return nullptr;
    }

    return it->second->at(case_x, case_y);
}

int SimuWorldScreen::handle_click(sf::Vector2f mouse_pos, DisplayManager* manager) {
    for (Tool* t : tools) {
        if (t->is_hovered(mouse_pos)) {
            t->handle_click(mouse_pos);
            if (t == stop_button && manager) {
                manager->draw_loading_screen();
                simulation->stop();
                Simulation::destroy();
                manager->set_screen(Screen_enum::Menu);
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f pos_visu = conv_coord_pixel_en_coord_realative_au_centre(mouse_pos);
        int case_x = pos_visu.x + centre_case_x;
        int case_y = pos_visu.y + centre_case_y;

        print_primaire("case:",case_x,"x",case_y);

        Case* c = get_case_from_carte_with_coord_world(case_x, case_y);

        if (c) {
            popup_case->set_case(c);
            popup_case->set_visible(true);
        } else {
            popup_case->set_visible(false);
        }
    }

    return -1;
}

void SimuWorldScreen::handle_camera_movement() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  centre_case_x--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) centre_case_x++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    centre_case_y--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  centre_case_y++;
}


void SimuWorldScreen::handle_zoom(float delta) {
    if (delta < 0 && largeur_visible_case_demi<20) { // Zoom avant = voir plus large
        print_secondaire("on zoom en arriere");
        largeur_visible_case_demi++;
        hauteur_visible_case_demi++;
    } else if (delta > 0 && largeur_visible_case_demi > 2 && hauteur_visible_case_demi > 2) {
        print_secondaire("on zoom en avant");
        largeur_visible_case_demi--;
        hauteur_visible_case_demi--;
    }
}


void SimuWorldScreen::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        handle_camera_movement();
    
        if (event.key.code == sf::Keyboard::Space && popup_case->is_visible()) {
            popup_case->fermer();
        }
    
        if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
            handle_zoom(0.1f);
        if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash)
            handle_zoom(-0.1f);
        update();
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        handle_zoom(event.mouseWheelScroll.delta > 0 ? 0.1f : -0.1f);
        update();
    }
}

void SimuWorldScreen::init_sprites() {
    
    // === TERRAIN ===
    const auto& terrains = TerrainManager::get_liste_terrain();
    for (const auto& name : terrains) {
        Terrain* terrain = TerrainManager::get(name);
        if (!terrain) continue;

        sf::Sprite sprite;
        std::string chemin = terrain->texture;
        sf::Texture* texture = new sf::Texture();
        if (!texture->loadFromFile(chemin)) {
            print_error("Erreur de chargement de la texture : ", chemin);
            delete texture;
            continue;
        }
        
        sprite.setTexture(*texture, true);
        sprites_reutilisables[name] = sprite;
        liste_textures.push_back(texture);
    }

    // === STRUCTURE ===
    auto textures_structure = StructureManager::get_all_structure_textures();
    for (const auto& [name, chemin] : textures_structure) {
        sf::Texture* texture = new sf::Texture();
        if (!texture->loadFromFile(chemin)) {
            print_error("Erreur de chargement de la texture structure : ", chemin);
            delete texture;
            continue;
        }
        sf::Sprite sprite;
        sprite.setTexture(*texture, true);
        sprites_reutilisables[name] = sprite;
        liste_textures.push_back(texture);
    }
}

sf::Vector2f SimuWorldScreen::calcul_scale_sprite() const {
    float scale_x = static_cast<float>(WINDOW_WIDTH)  / ((2 * largeur_visible_case_demi + 1) * SIZE_ORIGINE_X_TEXTURE);
    float scale_y = static_cast<float>(WINDOW_HEIGHT) / ((2 * hauteur_visible_case_demi + 1) * SIZE_ORIGINE_Y_TEXTURE_SANS_3D);
    return {scale_x, scale_y};
}

void SimuWorldScreen::update_sprites_scale() {
    sf::Vector2f scale = calcul_scale_sprite();
    for (auto& [name, sprite] : sprites_reutilisables) {
        sprite.setScale(scale);
    }
}

void SimuWorldScreen::update_case_size(){
    pixels_par_case_x = WINDOW_WIDTH / (2.0f * largeur_visible_case_demi+1);
    pixels_par_case_y = WINDOW_HEIGHT / (2.0f * hauteur_visible_case_demi+1);
}

void SimuWorldScreen::update(){
    actualiser_chunks_utilises();
    update_sprites_scale();
    update_case_size();
    print_chunk_charge();
}

sf::Vector2f SimuWorldScreen::conv_coord_pixel_en_coord_realative_au_centre(sf::Vector2f pos_pixel) const {
    float dx = pos_pixel.x - WINDOW_WIDTH / 2.0f;
    float dy = pos_pixel.y - WINDOW_HEIGHT / 2.0f;

    float case_x = (dx / (pixels_par_case_x / 2.0f) + dy / (pixels_par_case_y / 2.0f)) / 2.0f;
    float case_y = (dy / (pixels_par_case_y / 2.0f) - dx / (pixels_par_case_x / 2.0f)) / 2.0f;

    int x_arrondi = std::round(case_x);
    int y_arrondi = std::round(case_y);
    return sf::Vector2f(x_arrondi, y_arrondi);
}



sf::Vector2f SimuWorldScreen::conv_coord_visuel_en_coord_pixel(float case_x, float case_y) const {
    /*// on bouge le fond
    float pixel_x = (case_x-centre_case_x) * pixels_par_case_x + WINDOW_WIDTH / 2.0f;
    float pixel_y = (case_y-centre_case_y) * pixels_par_case_y + WINDOW_HEIGHT / 2.0f;
    */

    //on dessine  a l'ecran
    float pixel_x = (case_x) * pixels_par_case_x + WINDOW_WIDTH / 2.0f - pixels_par_case_x/2;
    float pixel_y = (case_y) * pixels_par_case_y + WINDOW_HEIGHT / 2.0f - pixels_par_case_y/2;

    return sf::Vector2f(pixel_x, pixel_y);
}

void SimuWorldScreen::draw_case_fond(sf::RenderWindow& window,std::string name_texture, float x, float y) const{
    auto it = sprites_reutilisables.find(name_texture);
    if (it == sprites_reutilisables.end()) {
        print_error("Texture introuvable : ", name_texture);
        return;
    }

    sf::Sprite sprite = it->second;
    sf::Vector2f pos_pixel = conv_coord_visuel_en_coord_pixel(x, y);
    
    sprite.setPosition(pos_pixel);
    window.draw(sprite);
}

Case* SimuWorldScreen::get_case_from_carte_with_coord_visuel(float dx, float dy) const {
    auto [world_x, world_y] = visu_to_monde(dx, dy);
    return get_case_from_carte_with_coord_world(world_x,world_y);
}

void SimuWorldScreen::draw_case_structure(sf::RenderWindow& window, Structure* s, float x, float y) const {
    std::string name_texture = s->get_name();

    auto it = sprites_reutilisables.find(name_texture);
    if (it == sprites_reutilisables.end()) {
        print_error("Texture structure introuvable : ", name_texture);
        return;
    }

    sf::Sprite sprite = it->second;
    sf::Vector2f pos_pixel = conv_coord_visuel_en_coord_pixel(x, y);

    float scale_ratio = std::min(pixels_par_case_x / s->get_taille_x(), pixels_par_case_y / s->get_taille_y());

    sprite.setScale(scale_ratio, scale_ratio);

    sf::FloatRect bounds = sprite.getGlobalBounds();
    // Tu veux que le centre du sprite soit à pos_pixel → donc tu déplaces le coin haut gauche :
    sprite.setPosition(
        pos_pixel.x + (pixels_par_case_x-bounds.width)/2 ,
        pos_pixel.y
    );

    window.draw(sprite);
}
