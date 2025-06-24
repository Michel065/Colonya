#ifndef _BUTTON_H
#define _BUTTON_H

#include "Tool.h"

class Button : public Tool {
public:
    Button(const std::string& text, const sf::Font& font, sf::Vector2f center, sf::Vector2f size,sf::Color color_but=sf::Color(100, 100, 200));

    void draw(sf::RenderWindow& window) const override;
    bool is_hovered(sf::Vector2f mouse_pos) const override;
    const std::string& get_label() const;

    void set_color(sf::Color color);

private:
    sf::RectangleShape shape;
    sf::Text label;
    std::string name;
};

#endif
