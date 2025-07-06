// ScrollableButtonList.h
#ifndef SCROLLABLE_BUTTON_LIST_H
#define SCROLLABLE_BUTTON_LIST_H

#include "../../Commun/includes.h"
#include "../../Commun/UIConfig.h"
#include "Tool.h"
#include "Button.h"

class ScrollableButtonList {
private:
    std::vector<std::string> labels;
    std::vector<Tool*> tools;
    const sf::Font& font;
    float start_y;
    int page_index = 0;
    int nb_par_page = 0;
    Button* selected_button = nullptr;

public:
    ScrollableButtonList(const sf::Font& font, float start_y = 150.f,int nb_par_page = 8);
    ~ScrollableButtonList();

    void add_label(const std::string& label);
    void update_display();
    void draw(sf::RenderWindow& window) const;
    void handle_click(sf::Vector2f mouse_pos);

    void clear();
    void reset_index();

    std::vector<Tool*>& get_tools();
    std::string get_selected_label() const;
    void delete_boutons(const std::string& label);

};

#endif
