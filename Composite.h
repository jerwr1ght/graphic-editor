#pragma once
#include "Figure.h"

class EventHandler;

class Composite : public Figure
{
private:
	int figures_count;
	Figure** figures;
	Composite(const Composite& another);
public:
	Composite();
	Composite(Figure* figure);
	~Composite();
	void add(Figure* figure);
	void remove_by_index(int index);
	void hide();
	void show();
	sf::FloatRect get_global_bounds();
	void set_scale(float x_scale, float y_scale);
	sf::Vector2f get_scale();
	bool check_intersection(Figure* another_figure);
	bool check_intersection(float x, float y);
	bool check_window_collision(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	void move(float x, float y);
	void set_position(float x, float y);
	void set_color(sf::Color color);
	sf::Color get_color();
	sf::Vector2f get_position();
	void reset();
	string get_string();
	Figure* clone() const;
	json serialize();
	void deserialize(json data);
};

