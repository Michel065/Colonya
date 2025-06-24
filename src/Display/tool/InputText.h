#ifndef _INPUT_TEXT_H
#define _INPUT_TEXT_H

#include "Tool.h"

class InputText : public Tool {
public:
    InputText(const std::string& label_text, const sf::Font& font, sf::Vector2f position, sf::Vector2f size, const std::string& default_value = "");

    void draw(sf::RenderWindow& window) const override;
    bool is_hovered(sf::Vector2f mouse_pos) const override;
    void handle_click(sf::Vector2f mouse_pos) override;
    void handle_text(sf::Uint32 unicode) override;
    void lose_focus() override;
    bool has_focus() const override;

    std::string get_value() const;
    void set_valid(bool ok);

private:
    sf::RectangleShape box;
    sf::Text label;
    sf::Text value_display;
    std::string value;
    bool focused = false;
};

#endif
