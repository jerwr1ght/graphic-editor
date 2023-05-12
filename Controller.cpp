#include "Controller.h"
#include <iostream>
#include "Memento.h"
#include "EventHandler.h"

Controller* Controller::controller_instance = nullptr;

Controller::Controller() 
{
	figures_count = 0;
	figures = new Figure * [figures_count];
	current_figure = nullptr;
	current_composite = nullptr;
}

Controller::~Controller() {
	clear();
	current_figure = nullptr;
	delete current_figure;
	current_composite = nullptr;
	delete current_composite;
	controller_instance = nullptr;
}

void Controller::add_figure(Figure* figure) {
	Figure** new_figures = new Figure * [figures_count + 1];
	for (int i = 0; i < figures_count; i++) {
		new_figures[i] = figures[i];
		figures[i] = nullptr;
		delete figures[i];
	}
	new_figures[figures_count++] = figure;

	delete[] figures;
	figures = new_figures;
	set_active_figure(figure);
}

void Controller::remove_figure_by_index(int index) {
	if (index < 0 || index >= figures_count)
		return;

	if (current_figure == figures[index])
		set_next_figure_active();
		
	int added_count = 0;
	Figure** new_figures = new Figure * [figures_count - 1];
	for (int i = 0; i < figures_count; i++) {
		if (i == index) {
			delete figures[i];
			continue;
		}

		new_figures[added_count++] = figures[i];
	}
	delete[] figures;
	figures = new_figures;

	figures_count--;
	if (is_empty())
		current_figure = nullptr;
}

bool Controller::is_empty() {
	return figures_count == 0;
}

void Controller::clear() {
	current_composite = nullptr;
	while (!is_empty()) {
		remove_figure_by_index(figures_count - 1);
	}
}

bool Controller::contains(Figure* figure) {
	return get_figure_index(figure) != -1;
}

int Controller::get_figure_index(Figure* figure) {
	for (int i = 0; i < figures_count; i++) {
		if (figure == figures[i])
			return i;
	}

	return -1;
}

Figure* Controller::get_active_figure() {
	return current_figure;
}

void Controller::set_active_figure(int index) {
	if (is_empty())
		return;
	
	if (index < 0)
		index = figures_count - 1;
	else if (index >= figures_count)
		index = 0;

	if (current_figure != nullptr) {
		current_figure->hide();
	}

	current_figure = figures[index];
	current_figure->show();
}

void Controller::set_active_figure(Figure* figure) {
	set_active_figure(get_figure_index(figure));
}

void Controller::set_next_figure_active() {
	set_active_figure(get_figure_index(current_figure) + 1);
}

void Controller::set_previous_figure_active() {
	set_active_figure(get_figure_index(current_figure) - 1);
}

Composite* Controller::get_composite() {
	return current_composite;
}

void Controller::set_composite(Composite* composite) {
	current_composite = composite;
}

void Controller::add_composite(Composite* composite) {
	add_figure(composite);
	set_composite(composite);
	set_active_figure(composite);
}

bool Controller::check_intersections_with_figure(Figure* figure) {
	bool result = false;
	for (int i = 0; i < figures_count; i++) {
		if (figures[i] != figure && figure->check_intersection(figures[i])) {
			result = true;
		}	
	}
	return result;
}

Controller* Controller::get_instance() {
	if (controller_instance == nullptr) {
		controller_instance = new Controller();
	}

	return controller_instance;
}

Memento* Controller::save(bool is_auto_move) {
	json state;

	state["is_auto_move"] = is_auto_move;

	state["figures_count"] = figures_count;
	std::vector<json> figures_json(figures_count);
	for (int i = 0; i < figures_count; i++) {
		figures_json[i] = figures[i]->serialize();
	}
	state["figures"] = figures_json;
	state["current_figure"] = get_figure_index(current_figure);

	return new Memento(state);
}

void Controller::restore(Memento* snapshot)
{	
	clear();

	json state = snapshot->get_state();

	int json_figures_count = state["figures_count"].get<int>();

	vector<json> state_figures(json_figures_count);

	state["figures"].get_to<vector<json>>(state_figures);
	
	for (int i = 0; i < json_figures_count; i++) {
		add_figure(EventHandler::create_figure_by_string_type(state_figures[i]["type"].get<string>()));
		figures[i]->deserialize(state_figures[i]);
	}

	current_figure = figures[state["current_figure"].get<int>()];
	current_figure->show();

}

