#include "Circle.h"

Circle::Circle(float radius, sf::Color color){
	default_color = color;
	circle = new sf::CircleShape(radius);
	set_color(color);
}

Circle::~Circle() {
	delete circle;
}

sf::FloatRect Circle::get_global_bounds() {
	return circle->getGlobalBounds();
}

void Circle::set_scale(float x_scale, float y_scale) {
	circle->setScale(x_scale, y_scale);
}

sf::Vector2f Circle::get_scale() {
	return circle->getScale();
}

void Circle::draw(sf::RenderWindow& window) {
	if (is_visible())
		window.draw(*circle);
};

void Circle::move(float x, float y) {
	circle->move(x, y);
}

void Circle::set_color(sf::Color color) {
	circle->setFillColor(color);
}

sf::Color Circle::get_color() {
	return circle->getFillColor();
}

sf::Vector2f Circle::get_position() {
	return circle->getPosition();
}

void Circle::set_position(float x, float y) {
	circle->setPosition(x, y);
}

string Circle::get_string() {
	return "Circle";
}

Circle::Circle(const Circle& another) {
	default_color = another.default_color;
	circle = new sf::CircleShape(another.circle->getRadius());
	set_position(another.circle->getPosition().x, another.circle->getPosition().y);
	set_color(another.circle->getFillColor());
	set_scale(another.circle->getScale().x, another.circle->getScale().y);
}

Figure* Circle::clone() const {
	return new Circle(*this);
}
