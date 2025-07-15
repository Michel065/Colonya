#ifndef _POPUP_CASE_H
#define _POPUP_CASE_H

#include "Tool.h"
#include "../../Commun/includes.h"
#include "../../Commun/UIConfig.h"
#include "../../Map/Case.h"

class Popup_Case : public Tool {
private:
    sf::RectangleShape background;
    sf::Text text;
    const sf::Font& font;
    bool visible = false;

public:
    Popup_Case(const sf::Font& font);
    void set_case(const Case* c); // Remplit le texte avec les infos de la case
    void set_visible(bool v);
    bool is_visible() const;
    void fermer();

    void draw(sf::RenderWindow& window) const override;
    bool is_hovered(sf::Vector2f) const override { return false; }
    void handle_click(sf::Vector2f) override {} // pas interactif
};

#endif
