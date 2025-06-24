#ifndef _TOOL_H
#define _TOOL_H

#include <SFML/Graphics.hpp>

class Tool {
public:
    virtual ~Tool() = default;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual bool is_hovered(sf::Vector2f mouse_pos) const = 0;
    virtual void handle_click(sf::Vector2f mouse_pos) {} // pour focus
    virtual void handle_text(sf::Uint32 unicode) {}      // pour saisie clavier
    virtual void handle_event(const sf::Event& event) {} // générique
    virtual void lose_focus() {}                         // quand on clique ailleurs
    virtual bool has_focus() const { return false; }
};

#endif
