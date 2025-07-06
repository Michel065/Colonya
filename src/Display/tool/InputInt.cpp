#include "InputInt.h"
#include <iostream>

InputInt::InputInt(const std::string& label_text, const sf::Font& font, sf::Vector2f center, sf::Vector2f size, int default_value)
{
    box.setSize(size);
    box.setOrigin(size.x / 2.f, size.y / 2.f);
    box.setPosition(center);
    box.setFillColor(sf::Color(220, 220, 220));

    label.setFont(font);
    label.setString(label_text);
    label.setCharacterSize(20);
    label.setFillColor(sf::Color::Black);
    label.setOrigin(label.getLocalBounds().width / 2.f, label.getLocalBounds().height / 2.f+10);
    label.setPosition(center.x, center.y - size.y / 1.5f);

    value_str = std::to_string(default_value);
    value_display.setFont(font);
    value_display.setCharacterSize(20);
    value_display.setFillColor(sf::Color::Black);
    value_display.setPosition(center.x - size.x / 2.f + 5, center.y - size.y / 2.f + 5);
    value_display.setString(value_str);
}

void InputInt::draw(sf::RenderWindow& window) const {
    window.draw(label);
    window.draw(box);
    window.draw(value_display);
}

bool InputInt::is_hovered(sf::Vector2f mouse_pos) const {
    return box.getGlobalBounds().contains(mouse_pos);
}

void InputInt::handle_click(sf::Vector2f mouse_pos) {
    focused = is_hovered(mouse_pos);
}

void InputInt::handle_text(sf::Uint32 unicode) {
    if (!focused) return;
    if (unicode == 8 && !value_str.empty()) {
        value_str.pop_back();
    } else if (unicode >= '0' && unicode <= '9') {
        value_str += static_cast<char>(unicode);
    }
    value_display.setString(value_str);
}

void InputInt::lose_focus() {
    focused = false;
}

bool InputInt::has_focus() const {
    return focused;
}

int InputInt::get_value() const {
    try {
        return std::stoi(value_str);
    } catch (...) {
        return 0;
    }
}

void InputInt::set_valid(bool valid) {
    box.setFillColor(valid ? sf::Color(220, 220, 220) : sf::Color(255, 150, 150));
}
std::string InputInt::get_value_str() const {
    return value_str;
}

bool InputInt::is_valid() {
    if (value_str.empty()) {
        set_valid(false);
        return false;
    }

    try {
        std::stoi(value_str);
    } catch (...) {
        set_valid(false);
        return false;
    }

    set_valid(true);
    return true;
}
