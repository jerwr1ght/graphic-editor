#include "Line.h"

Line::Line(float length, float thickness, sf::Color color) {
	default_color = color;
	line = new sf::RectangleShape(sf::Vector2f(length, thickness));
	set_color(color);
}

Line::~Line() {
	delete line;
}

sf::FloatRect Line::get_global_bounds() {
	return line->getGlobalBounds();
}

void Line::set_scale(float x_scale, float y_scale) {
	line->setScale(x_scale, y_scale);
}

sf::Vector2f Line::get_scale() {
	return line->getScale();
}

void Line::draw(sf::RenderWindow& window) {
	if (is_visible())
		window.draw(*line);
};

void Line::move(float x, float y) {
	line->move(x, y);
}

void Line::set_color(sf::Color color) {
	line->setFillColor(color);
}

sf::Color Line::get_color() {
	return line->getFillColor();
}

sf::Vector2f Line::get_position() {
	return line->getPosition();
}

void Line::set_position(float x, float y) {
	line->setPosition(x, y);
}

string Line::get_string() {
	return "Line";
}

Line::Line(const Line& another) {
	default_color = another.default_color;
	line = new sf::RectangleShape(another.line->getSize());
	set_position(another.line->getPosition().x, another.line->getPosition().y);
	set_color(another.line->getFillColor());
	set_scale(another.line->getScale().x, another.line->getScale().y);
}

Figure* Line::clone() const {
	return new Line(*this);
}