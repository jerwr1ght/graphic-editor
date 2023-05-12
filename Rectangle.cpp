#include "Rectangle.h"

Rectangle::Rectangle(float length, float width, sf::Color color) {
	default_color = color;
	rectangle = new sf::RectangleShape(sf::Vector2f(length, width));
	set_color(color);
}

Rectangle::~Rectangle() {
	delete rectangle;
}

sf::FloatRect Rectangle::get_global_bounds() {
	return rectangle->getGlobalBounds();
}

void Rectangle::set_scale(float x_scale, float y_scale) {
	rectangle->setScale(x_scale, y_scale);
}

sf::Vector2f Rectangle::get_scale() {
	return rectangle->getScale();
}

void Rectangle::draw(sf::RenderWindow& window) {
	if (is_visible())
		window.draw(*rectangle);
};

void Rectangle::move(float x, float y) {
	rectangle->move(x, y);
}

void Rectangle::set_color(sf::Color color) {
	rectangle->setFillColor(color);
}

sf::Color Rectangle::get_color() {
	return rectangle->getFillColor();
}

sf::Vector2f Rectangle::get_position() {
	return rectangle->getPosition();
}

void Rectangle::set_position(float x, float y) {
	rectangle->setPosition(x, y);
}

string Rectangle::get_string() {
	return "Rectangle";
}

Rectangle::Rectangle(const Rectangle& another) {
	default_color = another.default_color;
	rectangle = new sf::RectangleShape(another.rectangle->getSize());
	set_position(another.rectangle->getPosition().x, another.rectangle->getPosition().y);
	set_color(another.rectangle->getFillColor());
	set_scale(another.rectangle->getScale().x, another.rectangle->getScale().y);
}

Figure* Rectangle::clone() const {
	return new Rectangle(*this);
}

