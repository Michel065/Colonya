#include "InputText.h"

InputText::InputText(const std::string& label_text, const sf::Font& font, sf::Vector2f center, sf::Vector2f size, const std::string& default_value)
    : value(default_value)
{
    box.setSize(size);
    box.setOrigin(size.x / 2.f, size.y / 2.f);
    box.setPosition(center);
    box.setFillColor(sf::Color(220, 220, 220));

    label.setFont(font);
    label.setString(label_text);
    label.setCharacterSize(20);
    label.setFillColor(sf::Color::Black);
    label.setOrigin(label.getLocalBounds().width / 2.f, label.getLocalBounds().height / 2.f + 10);
    label.setPosition(center.x, center.y - size.y / 1.5f);

    value_display.setFont(font);
    value_display.setCharacterSize(20);
    value_display.setFillColor(sf::Color::Black);
    value_display.setString(value);
    value_display.setPosition(center.x - size.x / 2.f + 5, center.y - size.y / 2.f + 5);
}



void InputText::draw(sf::RenderWindow& window) const {
    window.draw(label);
    window.draw(box);
    window.draw(value_display);
}

bool InputText::is_hovered(sf::Vector2f mouse_pos) const {
    return box.getGlobalBounds().contains(mouse_pos);
}

void InputText::handle_click(sf::Vector2f mouse_pos) {
    focused = is_hovered(mouse_pos);
}

void InputText::handle_text(sf::Uint32 unicode) {
    if (!focused) return;
    if (unicode == 8) { // backspace
        if (!value.empty()) value.pop_back();
    } else if (unicode >= 32 && unicode <= 126) {
        value += static_cast<char>(unicode);
    }
    value_display.setString(value);
}

void InputText::lose_focus() {
    focused = false;
}

bool InputText::has_focus() const {
    return focused;
}

std::string InputText::get_value() const {
    return value;
}

void InputText::set_valid(bool ok) {
    box.setFillColor(ok ? sf::Color(200, 200, 200) : sf::Color(255, 100, 100));
}
