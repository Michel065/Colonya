#include "ScrollableButtonList.h"

ScrollableButtonList::ScrollableButtonList(const sf::Font& font, float start_y,int nb_par_page)
    : font(font), start_y(start_y),nb_par_page(nb_par_page) {}

ScrollableButtonList::~ScrollableButtonList() {
    for (Tool* t : tools) delete t;
}

void ScrollableButtonList::add_label(const std::string& label) {
    labels.push_back(label);
    update_display();
}

void ScrollableButtonList::delete_boutons(const std::string& label) {
    // Supprimer dans labels
    auto it_label = std::find(labels.begin(), labels.end(), label);
    if (it_label != labels.end()) {
        labels.erase(it_label);
    }
    update_display();
}

void ScrollableButtonList::update_display() {
    
    clear();

    int debut = page_index * nb_par_page;
    float y = start_y;

    for (int i = 0; i < nb_par_page && (debut + i) < (int)labels.size(); ++i) {
        const std::string& nom = labels[debut + i];

        sf::Text temp(nom, font, DEFAULT_FONT_SIZE_LABEL);
        float w = temp.getLocalBounds().width * 1.2f + 20.f;
        float h = temp.getLocalBounds().height + 20.f;

        Button* b = new Button(nom, font, {WINDOW_WIDTH / 2.f, y}, {w, h});
        b->set_color(sf::Color::White);
        tools.push_back(b);
        y += h + 10.f;
    }

    // Ajouter les boutons < et > pour la navigation
    tools.push_back(new Button("<", font, {100, WINDOW_HEIGHT / 2.f}, {40, 40}));
    tools.push_back(new Button(">", font, {WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2.f}, {40, 40}));
}

void ScrollableButtonList::draw(sf::RenderWindow& window) const {
    for (auto* tool : tools) tool->draw(window);
}

void ScrollableButtonList::handle_click(sf::Vector2f mouse_pos) {
    for (auto* tool : tools) {
        if (!tool->is_hovered(mouse_pos)) continue;
        tool->handle_click(mouse_pos);

        Button* b = dynamic_cast<Button*>(tool);
        if (!b) continue;

        std::string label = b->get_label();
        if (label == "<" && page_index > 0) {
            page_index--;
            update_display();
        } else if (label == ">") {
            int max_pages = (labels.size() + nb_par_page - 1) / nb_par_page;
            if (page_index < max_pages - 1) {
                page_index++;
                update_display();
            }
        } else if (std::find(labels.begin(), labels.end(), label) != labels.end()) {
            if (selected_button) selected_button->set_color(sf::Color::White);
            b->set_color(sf::Color(255, 165, 0));
            selected_button = b;
        }
        break;
    }
}

std::vector<Tool*>& ScrollableButtonList::get_tools() {
    return tools;
}

std::string ScrollableButtonList::get_selected_label() const {
    if (selected_button) return selected_button->get_label();
    return "";
}

void ScrollableButtonList::clear(){
    for (Tool* t : tools) delete t;
    tools.clear();
    selected_button = nullptr;
}

void ScrollableButtonList::reset_index(){
    page_index = 0;
}
