#include "Figure.h"

void Figure::set_trail_movement(bool status) {
	trail_movement = status;
}

bool Figure::get_trail_movement() {
	return trail_movement;
}

bool Figure::is_visible() {
	return visible;
}

void Figure::hide() {
	visible = false;
}

void Figure::show() {
	visible = true;
}

void Figure::auto_move(sf::RenderWindow& window, float elapsed_time) {
	move(1, sin(elapsed_time + 1));
}

void Figure::reset() {
	set_color(default_color);
	set_scale(1, 1);
}

bool Figure::check_intersection(Figure* another_figure) {
	bool result = get_global_bounds().intersects(another_figure->get_global_bounds());
	if (result)
		set_color(another_figure->get_color());

	return result;
}

bool Figure::check_intersection(float x, float y) {
	return get_global_bounds().contains(x, y);
}

bool Figure::check_window_collision(sf::RenderWindow& window) {
	if ((get_position().x < 0 || get_position().x > window.getSize().x - get_global_bounds().width)
		|| (get_position().y < 0 || get_position().y > window.getSize().y - get_global_bounds().height))
	{
		return true;
	}

	return false;
}

json Figure::serialize() {
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

	result["position"] = { {"x", get_position().x}, {"y", get_position().y} };
	result["scale"] = { {"x", get_scale().x}, {"y", get_scale().y} };
	result["visible"] = is_visible();
	result["trail"] = get_trail_movement();

	return result;
}

void Figure::deserialize(json data) {
	default_color = sf::Color(data["default_color"]["r"].get<int>(),
		data["default_color"]["g"].get<int>(),
		data["default_color"]["b"].get<int>());

	sf::Color current_color = sf::Color(data["color"]["r"].get<int>(),
		data["color"]["g"].get<int>(),
		data["color"]["b"].get<int>());

	set_color(current_color);

	set_scale(data["scale"]["x"].get<float>(),
		data["scale"]["y"].get<float>());

	set_position(data["position"]["x"].get<float>(),
		data["position"]["y"].get<float>());

	visible = data["visible"].get<bool>();

	set_trail_movement(data["trail"].get<bool>());
}