#pragma once
#include "Figure.h"
#include "Composite.h"

class Memento;
class EventHandler;

class Controller
{
private:
	int figures_count;
	Figure** figures;

	Figure* current_figure;
	Composite* current_composite;

	static Controller* controller_instance; // Частина патерну Singleton
public:
	Controller();
	~Controller();
	void add_figure(Figure* figure);
	void remove_figure_by_index(int index);
	bool contains(Figure* figure);
	int get_figure_index(Figure* figure);
	bool check_intersections_with_figure(Figure* figure);
	bool is_empty();
	void clear();

	// Робота з активною фігурою
	Figure* get_active_figure();
	void set_active_figure(int index);
	void set_active_figure(Figure* figure);
	void set_next_figure_active();
	void set_previous_figure_active();
	
	// Робота з композитом
	Composite* get_composite();
	void set_composite(Composite* composite);
	void add_composite(Composite* composite);

	// Робота зі статичним полем контролеру
	static Controller* get_instance();

	// Memento (роль творця знімків)
	Memento* save(bool is_auto_move);
	void restore(Memento* snapshot);

	
};

