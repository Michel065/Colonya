#ifndef _TEXT_LABEL_H
#define _TEXT_LABEL_H

#include "Tool.h"

class TextLabel : public Tool {
public:
    TextLabel(const std::string& text, const sf::Font& font, sf::Vector2f center, int size = 28, sf::Color color = sf::Color::Black);

    void draw(sf::RenderWindow& window) const override;
    bool is_hovered(sf::Vector2f) const override { return false; }
    std::string get_label() const;
    void set_text(std::string text);

private:
    sf::Text label;
};

#endif
