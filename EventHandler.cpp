#include "EventHandler.h"
using namespace std;
#include <iostream>
#include <fstream>

EventHandler::EventHandler(sf::Clock& _time, sf::Text& _label, 
	sf::Text& _status, sf::Sprite& _f1,  sf::Font& font) {
	trail.setPrimitiveType(sf::PrimitiveType::LinesStrip);
	time = _time;
	label = _label;
	status = _status;
	font.loadFromFile("ICTV-Bold.ttf");
	label.setFont(font);
	label.setCharacterSize(25);
	label.setFillColor(sf::Color::White);
	status.setFont(font);
	status.setCharacterSize(15);
	status.setFillColor(sf::Color::White);
	f1 = _f1;
	is_menu_enabled = false;
	is_figure_grabbed = false;
	is_auto_move = false;
	is_aggregation_enabled = false;
	saves_index = 0;
}

EventHandler::~EventHandler() {
	Controller::get_instance()->clear();
	trail.clear();
}

void EventHandler::poll_action_event(sf::Event& event, sf::RenderWindow& window) {
	if (!active_figure_exists())
		return;

	Figure* active_figure = Controller::get_instance()->get_active_figure();

	if (active_figure->check_window_collision(window)) {
		is_auto_move = false;
		set_status(Controller::get_instance()->get_active_figure()->get_string() + " stopped moving");
	}

	if (!is_aggregation_enabled) {
		if (Controller::get_instance()->check_intersections_with_figure(active_figure))
		set_status(active_figure->get_string() + " changed color");
	}
		
}

void EventHandler::poll_input_event(sf::Event& event, sf::RenderWindow& window) {

	switch (event.type) {
	case sf::Event::MouseWheelMoved:
		poll_mousewheel_event(event, window);
		break;
	case sf::Event::KeyPressed:
		set_mouse_cursor(sf::Cursor::Arrow, window);
		poll_keyboard_event(window);
		break;
	case sf::Event::MouseMoved:
		poll_mousemoved_event(window);
		break;
	case sf::Event::MouseButtonReleased:
		set_mouse_cursor(sf::Cursor::Arrow, window);
		break;
	default:
		break;
	}	
	
}

void EventHandler::poll_keyboard_event(sf::RenderWindow& window) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
		is_menu_enabled = !is_menu_enabled;
		return;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
	{
		return save_scene_as_json_file(SAVE_FILE_NAME);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) 
	{
		return load_scene_from_json_file(SAVE_FILE_NAME);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
		&& sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) 
	{	
		return delete_saves_json_file(SAVE_FILE_NAME);
	}


	Figure* temp = nullptr;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		temp = new Circle(CIRCLE_SIZE, get_random_color());
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		temp = new Line(LINE_SIZE.x, LINE_SIZE.y, get_random_color());
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		temp = new Rectangle(RECTANGLE_SIZE.x, RECTANGLE_SIZE.y, get_random_color());
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		temp = new Square(SQUARE_SIZE, get_random_color());
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		temp = new Star(get_random_color());
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		temp = new Triangle(TRIANGLE_SIZE, get_random_color());
	
	if (temp) {
		Controller::get_instance()->add_figure(temp);
		set_status("Created " + temp->get_string());
	}
	
	if (!active_figure_exists())
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		Controller::get_instance()->set_previous_figure_active();
		set_status("Selected " + Controller::get_instance()->get_active_figure()->get_string());
	}	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		Controller::get_instance()->set_next_figure_active();
		set_status("Selected " + Controller::get_instance()->get_active_figure()->get_string());
	}	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		Controller::get_instance()->get_active_figure()->reset();
		set_status("Reseted " + Controller::get_instance()->get_active_figure()->get_string());
	}	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
		Controller::get_instance()->get_active_figure()->set_trail_movement(
			!(Controller::get_instance()->get_active_figure()->get_trail_movement() == true));
		string res = Controller::get_instance()->get_active_figure()->get_trail_movement() ? "enabled" : "disabled";
		set_status("Trail " + res);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		Controller::get_instance()->get_active_figure()->set_color(get_random_color());
		set_status(Controller::get_instance()->get_active_figure()->get_string() + " color changed");
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
		if (Controller::get_instance()->get_active_figure()->is_visible()) {
			Controller::get_instance()->get_active_figure()->hide();
			set_status(Controller::get_instance()->get_active_figure()->get_string() + " hidden");
		}
		else {
			Controller::get_instance()->get_active_figure()->show();
			set_status(Controller::get_instance()->get_active_figure()->get_string() + " shown");
		}
			
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
		is_auto_move = !(is_auto_move);
		string res = is_auto_move ? " moved auto" : " stopped moving";
		set_status(Controller::get_instance()->get_active_figure()->get_string() + res);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
		if (!is_aggregation_enabled) {
			is_aggregation_enabled = true;
			Controller::get_instance()->add_composite(new Composite(Controller::get_instance()->get_active_figure()));
			set_status("Composite created");
		}
		else if (is_aggregation_enabled && Controller::get_instance()->get_active_figure() != Controller::get_instance()->get_composite()) {
			Controller::get_instance()->get_composite()->add(Controller::get_instance()->get_active_figure());
			Controller::get_instance()->set_active_figure(Controller::get_instance()->get_composite());
			set_status("Added " + Controller::get_instance()->get_active_figure()->get_string() + " to Composite");
		}
		else {
			is_aggregation_enabled = false;
			Controller::get_instance()->set_composite(nullptr);
			set_status("Composite saved");
		}

	}
		
	
}

void EventHandler::poll_mousewheel_event(sf::Event& event, sf::RenderWindow& window) {
	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
		active_figure_exists()) 
	{
		set_mouse_cursor(sf::Cursor::SizeTopLeftBottomRight, window);
		Figure* figure = Controller::get_instance()->get_active_figure();
		figure->set_scale(figure->get_scale().x + event.mouseWheel.delta,
			figure->get_scale().y + event.mouseWheel.delta);

		
		set_status("Scaled " + Controller::get_instance()->get_active_figure()->get_string());
	}

	
}

void EventHandler::poll_mousemoved_event(sf::RenderWindow& window) {
	if (!active_figure_exists())
		return;

	float x = (float)sf::Mouse::getPosition(window).x;
	float y = (float)sf::Mouse::getPosition(window).y;

	if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) && is_figure_grabbed) ||
		(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
			Controller::get_instance()->get_active_figure()->check_intersection(x, y)))
	{
		is_figure_grabbed = true;
		Controller::get_instance()->get_active_figure()->set_position(x - Controller::get_instance()->get_active_figure()->get_global_bounds().width / 2,
			y - Controller::get_instance()->get_active_figure()->get_global_bounds().height / 2);

		set_status("Grabbed " + Controller::get_instance()->get_active_figure()->get_string());
		
		set_mouse_cursor(sf::Cursor::Hand, window);
	}
	else {
		set_mouse_cursor(sf::Cursor::Arrow, window);
		is_figure_grabbed = false;
	}
	
}

void EventHandler::poll_draw_event(sf::RenderWindow& window) {
	
	// Розставляємо усі елементи на екрані
	window.clear(sf::Color(66, 112, 63));
	label.setPosition((window.getSize().x - label.getGlobalBounds().width) / 2.f, 5);
	status.setPosition(6, (float) window.getSize().y - 26);
	window.draw(label);
	window.draw(status);

	// Малюємо фігури, слід та написи, рухаємо фігуру
	if (active_figure_exists()) {
		label.setString("<" + Controller::get_instance()->get_active_figure()->get_string() + ">");

		// Малюємо слід, якщо користувач натиснув необхідну клавішу
		if (Controller::get_instance()->get_active_figure()->get_trail_movement()) {
			sf::FloatRect bounds = Controller::get_instance()->get_active_figure()->get_global_bounds();
			trail.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2)));
			window.draw(trail);
		}
		else
			trail.clear();
		
		// Рух за y = sin(x * t), де t - час від початку руху
		if (is_auto_move) {
			Controller::get_instance()->get_active_figure()->auto_move(window, time.getElapsedTime().asSeconds());
		}
		else
			time.restart();

		// Малюємо фігуру та (або) композит
		Controller::get_instance()->get_active_figure()->draw(window);
		if (active_composite_exists()) {
			Controller::get_instance()->get_composite()->show();
			Controller::get_instance()->get_composite()->draw(window);
			label.setString("<" 
				+ Controller::get_instance()->get_active_figure()->get_string() 
				+ "/Composite Mode>");
		}
	}
	else {
		label.setString("<Selected figure>");
	}

	// Виводимо на екран меню за необхідності
	if (is_menu_enabled)
		window.draw(f1);

	window.display();
}

bool EventHandler::active_figure_exists() {
	return (Controller::get_instance()->get_active_figure() != nullptr);
}

bool EventHandler::active_composite_exists() {
	return (Controller::get_instance()->get_composite() != nullptr);
}

sf::Color EventHandler::get_random_color() {
	return sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
}

void EventHandler::set_mouse_cursor(sf::Cursor::Type type, sf::RenderWindow& window) {
	sf::Cursor mouse_cursor;
	mouse_cursor.loadFromSystem(type);
	window.setMouseCursor(mouse_cursor);
}

void EventHandler::set_status(const sf::String& status_message) {
	status.setString(status_message);
}

Figure* EventHandler::create_figure_by_string_type(string type) {
	sf::Color default_color = sf::Color::Black;
	Figure* result = nullptr;

	if (type == "Triangle") {
		result = new Triangle(TRIANGLE_SIZE, default_color);
	}
	else if (type == "Circle") {
		result = new Circle(CIRCLE_SIZE, default_color);
	}
	else if (type == "Line") {
		result = new Line(LINE_SIZE.x, LINE_SIZE.y, default_color);
	}
	else if (type == "Rectangle") {
		result = new Rectangle(RECTANGLE_SIZE.x, RECTANGLE_SIZE.y, default_color);
	}
	else if (type == "Square") {
		result = new Square(SQUARE_SIZE, default_color);
	}
	else if (type == "Star") {
		result = new Star(default_color);
	}
	else if (type == "Composite") {
		result = new Composite();
	}

	return result;
}

void EventHandler::delete_saves_json_file(string file_name) {
	remove(file_name.c_str());
	set_status("Saves file deleted");
}

void EventHandler::save_scene_as_json_file(string file_name) {
	if (Controller::get_instance()->is_empty())
		return set_status("Couldn't save empty scene");

	json hisory;
	int saves_count = 0;
	
	ifstream in;
	in.open(file_name);
	if (in.is_open()) {
		in >> hisory;
		saves_count = hisory["saves_count"].get<int>();
	}

	in.close();

	Memento* snapshot = Controller::get_instance()->save(is_auto_move);
	
	hisory["saves_count"] = ++saves_count;
	hisory["saves"].push_back(snapshot->get_state());
	
	ofstream out;
	out.open(file_name, ios::out);
	
	out << hisory;
	
	out.close();

	saves_index = saves_count - 1;

	return set_status("Saved scene as file");
}

void EventHandler::load_scene_from_json_file(string file_name) {
	ifstream in;
	in.open(file_name);

	if (!in.is_open()) {
		return set_status("Couldn't loaded from file");
	}

	trail.clear();

	json history;
	
	in >> history;

	int saves_count = history["saves_count"].get<int>();

	if (saves_index < 1)
		saves_index = saves_count - 1;
	else
		saves_index--;

	Memento* snapshot = new Memento(history["saves"][saves_index]);
	Controller::get_instance()->restore(snapshot);
	
	is_auto_move = snapshot->get_state()["is_auto_move"].get<bool>();
	is_aggregation_enabled = false;

	set_status("Loaded scene from file");

	return in.close();
}
