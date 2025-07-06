#include "TextLabel.h"

TextLabel::TextLabel(const std::string& text, const sf::Font& font, sf::Vector2f center, int size, sf::Color color) {
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    label.setPosition(center);
}

void TextLabel::draw(sf::RenderWindow& window) const {
    window.draw(label);
}

std::string TextLabel::get_label() const {
    return label.getString();
}

void TextLabel::set_text(std::string text){
    label.setString(text);
}