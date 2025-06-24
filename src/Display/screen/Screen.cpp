#include "Screen.h"

Screen::Screen(const sf::Font& font, Screen_enum id) 
    : font(font), id(id) {}

Screen_enum Screen::get_id() {
    return id;
}

bool Screen::is_active(Screen_enum& state) {
    return state == id;
}