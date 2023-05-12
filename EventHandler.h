#pragma once
#include "Controller.h"
#include "Line.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Star.h"
#include "Square.h"
#include "Composite.h"
#include "Memento.h"

// json-файл для збереження сцени
const string SAVE_FILE_NAME = "saves.json";

// Константні розміри фігур
const sf::Vector2f LINE_SIZE(70, 5);
const sf::Vector2f RECTANGLE_SIZE(20, 10);
const float CIRCLE_SIZE = 15;
const float SQUARE_SIZE = 20;
const float TRIANGLE_SIZE = 20;

class EventHandler
{
private:
	sf::Clock time;
	sf::Text label;
	sf::Text status;
	sf::VertexArray trail;
	sf::Sprite f1;
	
	bool is_menu_enabled;
	bool is_figure_grabbed;
	bool is_auto_move;
	bool is_aggregation_enabled;

	// Збереження сцени
	int saves_index; // Індекс вибраного у колекції знімку, що збережений у файлі
	void delete_saves_json_file(string file_name);
	void save_scene_as_json_file(string file_name);
	void load_scene_from_json_file(string file_name);


	static sf::Color get_random_color();
	void set_mouse_cursor(sf::Cursor::Type type, sf::RenderWindow& window);
	void set_status(const sf::String& status_message);
public:
	EventHandler(sf::Clock& _time, sf::Text& _label, 
		sf::Text& _status, sf::Sprite& _f1, sf::Font& _font);
	~EventHandler();
	
	// Методи для роботи з діями користувача
	void poll_input_event(sf::Event& event, sf::RenderWindow& window);
	void poll_keyboard_event(sf::RenderWindow& window);
	void poll_mousewheel_event(sf::Event& event, sf::RenderWindow& window);
	void poll_mousemoved_event(sf::RenderWindow& window);
	void poll_action_event(sf::Event& event, sf::RenderWindow& window);

	// Draw events
	void poll_draw_event(sf::RenderWindow& window);
	bool active_figure_exists();
	bool active_composite_exists();

	// Для десеріалізації та серіалізації
	static Figure* create_figure_by_string_type(string type);
};

