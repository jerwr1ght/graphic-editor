#include "Square.h"

Square::Square(float side, sf::Color color) {
	default_color = color;
	square = new sf::RectangleShape(sf::Vector2f(side, side));
	set_color(color);
}

Square::~Square() {
	delete square;
}

sf::FloatRect Square::get_global_bounds() {
	return square->getGlobalBounds();
}

void Square::set_scale(float x_scale, float y_scale) {
	square->setScale(x_scale, y_scale);
}

sf::Vector2f Square::get_scale() {
	return square->getScale();
}

void Square::draw(sf::RenderWindow& window) {
	if (is_visible())
		window.draw(*square);
};

void Square::move(float x, float y) {
	square->move(x, y);
}

void Square::set_color(sf::Color color) {
	square->setFillColor(color);
}

sf::Color Square::get_color() {
	return square->getFillColor();
}

sf::Vector2f Square::get_position() {
	return square->getPosition();
}

void Square::set_position(float x, float y) {
	square->setPosition(x, y);
}

string Square::get_string() {
	return "Square";
}

Square::Square(const Square& another) {
	default_color = another.default_color;
	square = new sf::RectangleShape(another.square->getSize());
	set_position(another.square->getPosition().x, another.square->getPosition().y);
	set_color(another.square->getFillColor());
	set_scale(another.square->getScale().x, another.square->getScale().y);
}

Figure* Square::clone() const {
	return new Square(*this);
}