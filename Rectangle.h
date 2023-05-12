#pragma once
#include "Figure.h"
class Rectangle : public Figure
{
private:
	sf::RectangleShape* rectangle;
	Rectangle(const Rectangle& another);
public:
	Rectangle(float length, float width, sf::Color color);
	~Rectangle();
	sf::FloatRect get_global_bounds();
	void set_scale(float x_scale, float y_scale);
	sf::Vector2f get_scale();
	void draw(sf::RenderWindow& window);
	void move(float x, float y);
	void set_position(float x, float y);
	void set_color(sf::Color color);
	sf::Color get_color();
	sf::Vector2f get_position();
	string get_string();
	Figure* clone() const;
};

