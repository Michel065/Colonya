#include "Popup_Case.h"

Popup_Case::Popup_Case(const sf::Font& font) : font(font) {
    background.setSize(sf::Vector2f(300, 120));
    background.setFillColor(sf::Color(255, 255, 255, 230));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.f);
    background.setPosition(20.f, 20.f); // collé à droite

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(30.f, 30.f);
}

void Popup_Case::set_case(const Case* c) {
    if (!c) return;
    std::string contenu;

    contenu += "Terrain    : " + c->get_terrain()->get_name() + "\n";
    contenu += "Ressource  : " + (c->get_ressource() ? c->get_ressource()->get_name() : "vide") + "\n";
    contenu += "Structure  : " + (c->get_structure() ? c->get_structure()->get_name() : "vide");

    text.setString(contenu);
}

void Popup_Case::set_visible(bool v) {
    visible = v;
}

bool Popup_Case::is_visible() const {
    return visible;
}

void Popup_Case::fermer() {
    visible = false;
}

void Popup_Case::draw(sf::RenderWindow& window) const {
    if (!visible) return;
    window.draw(background);
    window.draw(text);
}
