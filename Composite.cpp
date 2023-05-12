#include "Composite.h"
using namespace std;
#include <iostream>
#include "EventHandler.h"

Composite::Composite(Figure* figure) {
    figures_count = 0;
    figures = new Figure* [figures_count];
    add(figure);
}

Composite::Composite() {
    figures_count = 0;
    figures = new Figure * [figures_count];
}

Composite::~Composite() {
    while(figures_count > 0)
    {
        remove_by_index(0);
    }
    delete[] figures;
}

void Composite::add(Figure* figure) {

    Figure** new_figures = new Figure* [figures_count + 1];
    for (int i = 0; i < figures_count; i++) {
        new_figures[i] = figures[i];
        figures[i] = nullptr;
        delete figures[i];
    }
    new_figures[figures_count++] = figure->clone();

    delete[] figures;
    figures = new_figures;
}

void Composite::remove_by_index(int index) {
    if (index < 0 || index >= figures_count)
        return;

    int added_count = 0;
    Figure** new_figures = new Figure* [figures_count - 1];
    for (int i = 0; i < figures_count; i++) {
        if (i != index) {
            new_figures[added_count++] = figures[i];
            figures[i] = nullptr;
        }
 
        delete figures[i];
    }
    delete[] figures;
    figures = new_figures;

    figures_count--;
}

void Composite::hide() {
    visible = false;
	for (int i = 0; i < figures_count; i++) {
		figures[i]->hide();
	}
}

void Composite::show() {
    visible = true;
	for (int i = 0; i < figures_count; i++) {
		figures[i]->show();
	}
}

sf::FloatRect Composite::get_global_bounds() {
    sf::FloatRect composite_bounds, figure_bounds;

    composite_bounds = (figures_count <= 0) ? composite_bounds : figures[0]->get_global_bounds();

    float right_up = composite_bounds.left + composite_bounds.width;
    float left_down = composite_bounds.top + composite_bounds.height;


    for (int i = 1; i < figures_count; i++)
    {
        figure_bounds = figures[i]->get_global_bounds();


        if (right_up < figure_bounds.left + figure_bounds.width)
        {
            right_up = figure_bounds.left + figure_bounds.width;
        }

        if (left_down < figure_bounds.top + figure_bounds.height)
        {
            left_down = figure_bounds.top + figure_bounds.height;
        }

        if (composite_bounds.left > figure_bounds.left)
        {
            composite_bounds.left = figure_bounds.left;
        }

        if (composite_bounds.top > figure_bounds.top)
        {
            composite_bounds.top = figure_bounds.top;
        }


    }
    composite_bounds.width = right_up - composite_bounds.left;
    composite_bounds.height = left_down - composite_bounds.top;


    return composite_bounds;
}

void Composite::set_scale(float x_scale, float y_scale) {
    for (int i = 0; i < figures_count; i++) {
        figures[i]->set_scale(x_scale, y_scale);
    }
}

sf::Vector2f Composite::get_scale() {
    return (figures_count <= 0) ? sf::Vector2f(0, 0) : figures[0]->get_scale();
}

bool Composite::check_intersection(Figure* another_figure) {

    bool result = false;
    for (int i = 0; i < figures_count; i++) {
        if (figures[i]->check_intersection(another_figure))
            result = true;
    }
    return result;
}                                                               

bool Composite::check_intersection(float x, float y) {
    for (int i = 0; i < figures_count; i++) {
        if (figures[i]->check_intersection(x, y))
            return true;
    }

    return get_global_bounds().contains(x, y);
}

bool Composite::check_window_collision(sf::RenderWindow& window) {
    for (int i = 0; i < figures_count; i++) {
        if (figures[i]->check_window_collision(window))
        {
            return true;
        }
    }
    
    return false;
}

void Composite::draw(sf::RenderWindow& window) {
    if (!is_visible())
        return;

    for (int i = 0; i < figures_count; i++) {
        figures[i]->show();
        figures[i]->draw(window);
    }
}

void Composite::move(float x, float y) {
    for (int i = 0; i < figures_count; i++) {
        figures[i]->move(x, y);
    }
}

void Composite::set_position(float x, float y) {
    move(x - get_position().x, y - get_position().y);
}

void Composite::set_color(sf::Color color) {
    for (int i = 0; i < figures_count; i++) {
        figures[i]->set_color(color);
    }
}

sf::Color Composite::get_color() {
    return (figures_count <= 0) ? default_color : figures[0]->get_color();
}

sf::Vector2f Composite::get_position() {
    sf::FloatRect global_bounds = get_global_bounds();
    return sf::Vector2f(global_bounds.left + global_bounds.width / 2,
        global_bounds.top + global_bounds.height / 2);
}

void Composite::reset() {
    for (int i = 0; i < figures_count; i++) {
        figures[i]->reset();
    }
}

string Composite::get_string() {
    return "Composite";
}

Composite::Composite(const Composite& another) {
    default_color = another.default_color;
    Figure* temp;
    Figure* another_temp;
    for (int i = 0; i < another.figures_count; i++) {
        another_temp = another.figures[i];
        add(another_temp);
        
        temp = figures[figures_count - 1];
        temp->set_position(another_temp->get_position().x, another_temp->get_position().y);
        temp->set_color(another_temp->get_color());
        temp->set_scale(another_temp->get_scale().x, another_temp->get_scale().y);
    }

    temp = nullptr;
    delete temp;
    another_temp = nullptr;
    delete another_temp;
    
}

Figure* Composite::clone() const {
    return new Composite(*this);
}

json Composite::serialize() {
    json result;

    result["type"] = get_string();

    result["default_color"] = {
        {"r", default_color.r},
        {"g", default_color.g},
        {"b", default_color.b}
    };
    result["color"] = {
        {"r", get_color().r},
        {"g", get_color().g},
        {"b", get_color().b}
    };


    result["figures_count"] = figures_count;
    std::vector<json> figures_json(figures_count);
    for (int i = 0; i < figures_count; i++) {
        figures_json[i] = figures[i]->serialize();
    }
    result["figures"] = figures_json;
    result["visible"] = is_visible();
    result["trail"] = get_trail_movement();

    return result;
}

void Composite::deserialize(json data) {
    default_color = sf::Color(data["default_color"]["r"].get<int>(),
        data["default_color"]["g"].get<int>(),
        data["default_color"]["b"].get<int>());

    int json_figures_count = data["figures_count"].get<int>();

    std::vector<json> json_figures(json_figures_count);
    data["figures"].get_to<vector<json>>(json_figures);
    for (int i = 0; i < json_figures_count; i++) {
        add(EventHandler::create_figure_by_string_type(json_figures[i]["type"].get<string>()));
        figures[i]->deserialize(json_figures[i]);
    } 

    visible = data["visible"].get<bool>();

    set_trail_movement(data["trail"].get<bool>());

    
}


