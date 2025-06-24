#include "Button.h"

Button::Button(const std::string& text, const sf::Font& font, sf::Vector2f center, sf::Vector2f size,sf::Color color_but)
    : name(text)
{
    shape.setSize(size);
    shape.setOrigin(size.x / 2.f, size.y / 2.f);  // centré
    shape.setPosition(center);
    shape.setFillColor(color_but);

    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(24);
    label.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    label.setPosition(center);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}

bool Button::is_hovered(sf::Vector2f mouse_pos) const {
    return shape.getGlobalBounds().contains(mouse_pos);
}

const std::string& Button::get_label() const {
    return name;
}

void Button::set_color(sf::Color color){
    shape.setFillColor(color);
}