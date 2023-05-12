#pragma once
#include "Figure.h"
class Circle : public Figure
{
private:
	sf::CircleShape* circle;
	Circle(const Circle& another);
public:
	Circle(float radius, sf::Color color);
	~Circle();
	sf::FloatRect get_global_bounds();
	void set_scale(float x_scale, float y_scale);
	sf::Vector2f get_scale();
	void draw(sf::RenderWindow& window);
	void move(float x, float y);
	void set_position(float x, float y);
	void set_color(sf::Color color);
	sf::Vector2f get_position();
	sf::Color get_color();
	string get_string();
	Figure* clone() const;
};

