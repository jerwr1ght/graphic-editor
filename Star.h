#pragma once
#include "Figure.h"
class Star : public Figure
{
private:
	sf::ConvexShape* star;
	void set_points();
	Star(const Star& another);
public:
	Star(sf::Color color);
	~Star();
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

