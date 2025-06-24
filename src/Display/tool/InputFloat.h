#ifndef _INPUT_FLOAT_H
#define _INPUT_FLOAT_H

#include "Tool.h"

class InputFloat : public Tool {
public:
    InputFloat(const std::string& label_text, const sf::Font& font, sf::Vector2f center, sf::Vector2f size, int default_value = 0);

    void draw(sf::RenderWindow& window) const override;
    bool is_hovered(sf::Vector2f mouse_pos) const override;
    void handle_click(sf::Vector2f mouse_pos) override;
    void handle_text(sf::Uint32 unicode) override;
    void lose_focus() override;
    bool has_focus() const override;

    float get_value() const;

    void set_valid(bool valid);
    std::string get_value_str() const;

private:
    sf::RectangleShape box;
    sf::Text label;
    sf::Text value_display;
    std::string value_str;
    bool focused = false;
};

#endif
