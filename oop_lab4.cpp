#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
using namespace std;
#include "EventHandler.h"
using namespace sf;

int main()
{
    srand((unsigned int)time(NULL));

    ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    sf::Image icon;
    icon.loadFromFile("icon.png");
    sf::Texture f1_texture;
    f1_texture.loadFromFile("f1.png");
    sf::Sprite f1;
    f1.setTexture(f1_texture, true);
    sf::RenderWindow window(VideoMode(1000, 800), 
        "You want to play? Let's play", 
        sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    sf::Text label;
    sf::Clock time;
    sf::Font font;
    sf::Text status;
    EventHandler event_handler(time, label, status, f1, font);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            event_handler.poll_input_event(event, window); 
        }

        event_handler.poll_action_event(event, window);
        event_handler.poll_draw_event(window);
    }
    
    
    return 0;
    
}