#include "Triangle.h"
#include <iostream>

Triangle::Triangle(float size, sf::Color color) {
	default_color = color;
	triangle = new sf::CircleShape(size, 3);
	set_color(color);
}

Triangle::~Triangle() {
	delete triangle;
}

sf::FloatRect Triangle::get_global_bounds() {
	return triangle->getGlobalBounds();
}

void Triangle::set_scale(float x_scale, float y_scale) {
	triangle->setScale(x_scale, y_scale);
}

sf::Vector2f Triangle::get_scale() {
	return triangle->getScale();
}

void Triangle::draw(sf::RenderWindow& window) {
	if (is_visible())
		window.draw(*triangle);
};

void Triangle::move(float x, float y) {
	triangle->move(x, y);
}

void Triangle::set_color(sf::Color color) {
	triangle->setFillColor(color);
}

sf::Color Triangle::get_color() {
	return triangle->getFillColor();
}

sf::Vector2f Triangle::get_position() {
	return triangle->getPosition();
}

void Triangle::set_position(float x, float y) {
	triangle->setPosition(x, y);
}

string Triangle::get_string() {
	return "Triangle";
}

Triangle::Triangle(const Triangle& another) {
	default_color = another.default_color;
	triangle = new sf::CircleShape(another.triangle->getRadius(), 3);
	set_position(another.triangle->getPosition().x, another.triangle->getPosition().y);
	set_color(another.triangle->getFillColor());
	set_scale(another.triangle->getScale().x, another.triangle->getScale().y);
}

Figure* Triangle::clone() const {
	return new Triangle(*this);
}