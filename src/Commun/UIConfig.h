#ifndef _UI_CONFIG_H
#define _UI_CONFIG_H

#include <SFML/System.hpp>

// Fenêtre
constexpr unsigned int WINDOW_WIDTH  = 1280;
constexpr unsigned int WINDOW_HEIGHT = 720;
constexpr const char* WINDOW_TITLE   = "Colonya";

// Grille / logique
constexpr float SIZE_ORIGINE_X_TEXTURE = 64.f;
constexpr float SIZE_ORIGINE_Y_TEXTURE = 40.f;
constexpr float SIZE_ORIGINE_Y_TEXTURE_SANS_3D = 32.f;


// Interface graphique
constexpr float BUTTON_WIDTH  = 200.f;
constexpr float BUTTON_HEIGHT = 60.f;

constexpr float INPUT_WIDTH  = 250.f;
constexpr float INPUT_HEIGHT = 40.f;

constexpr float UI_VERTICAL_SPACING   = 70.f;   // espace vertical entre deux inputs
constexpr float UI_HORIZONTAL_SPACING = 50.f;   // entre deux colonnes si besoin

constexpr float UI_SECTION_MARGIN_TOP = 150.f;  // distance du haut pour le début de section

constexpr float DEFAULT_FONT_SIZE_LABEL = 20.f;
constexpr float DEFAULT_FONT_SIZE_TITLE = 40.f;


#endif
