#include "Star.h"

Star::Star(sf::Color color) {
	default_color = color;
	star = new sf::ConvexShape(10);
	set_points();
	set_color(color);
}

void Star::set_points() {
	star->setPoint(0, sf::Vector2f(60, 0));
	star->setPoint(1, sf::Vector2f(72, 40));
	star->setPoint(2, sf::Vector2f(110, 40));
	star->setPoint(3, sf::Vector2f(80, 60));
	star->setPoint(4, sf::Vector2f(100, 100));
	star->setPoint(5, sf::Vector2f(60, 70));
	star->setPoint(6, sf::Vector2f(20, 100));
	star->setPoint(7, sf::Vector2f(40, 60));
	star->setPoint(8, sf::Vector2f(10, 40));
	star->setPoint(9, sf::Vector2f(48, 40));
}

Star::~Star() {
	delete star;
}

sf::FloatRect Star::get_global_bounds() {
	return star->getGlobalBounds();
}

void Star::set_scale(float x_scale, float y_scale) {
	star->setScale(x_scale, y_scale);
}

sf::Vector2f Star::get_scale() {
	return star->getScale();
}

void Star::draw(sf::RenderWindow& window) {
	if (is_visible())
		window.draw(*star);
};

void Star::move(float x, float y) {
	star->move(x, y);
}

void Star::set_color(sf::Color color) {
	star->setFillColor(color);
}

sf::Color Star::get_color() {
	return star->getFillColor();
}

sf::Vector2f Star::get_position() {
	return star->getPosition();
}

void Star::set_position(float x, float y) {
	star->setPosition(x, y);
}

string Star::get_string() {
	return "Star";
}

Star::Star(const Star& another) {
	default_color = another.default_color;
	star = new sf::ConvexShape(10);
	set_points();
	set_position(another.star->getPosition().x, another.star->getPosition().y);
	set_color(another.star->getFillColor());
	set_scale(another.star->getScale().x, another.star->getScale().y);
	
}

Figure* Star::clone() const {
	return new Star(*this);
}