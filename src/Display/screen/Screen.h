#ifndef _SCREEN_H
#define _SCREEN_H

#include "../../Commun/includes.h"
#include "../Screen_enum.h"

class DisplayManager;

class Screen {
public:
    Screen(const sf::Font& font,Screen_enum id=Screen_enum::NonDef);
    virtual ~Screen() = default;

    virtual void draw(sf::RenderWindow& window) const=0;
    virtual int handle_click(sf::Vector2f mouse_pos,DisplayManager*manager=nullptr)=0;
    virtual void handle_event(const sf::Event& event)=0;

    Screen_enum get_id();

    bool is_active(Screen_enum& state);
    
protected:
    const sf::Font& font;
    Screen_enum id;
};

#endif