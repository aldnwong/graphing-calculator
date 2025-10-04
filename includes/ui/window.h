#ifndef ALDNWONG_WINDOW_H
#define ALDNWONG_WINDOW_H
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "system.h"
#include "constants.h"
#include "../list_iterated/list_iterated.h"

using namespace std;

class Window {
public:
    // Constructor
    Window(unsigned int WINDOW_WIDTH = 1260U, unsigned int WINDOW_HEIGHT = 720U, unsigned int FRAME_RATE = 60U);

    // Runs window
    void run();

private:
    // Backend system for grid and graphing
    System system;

    // UI elements
    sf::RenderWindow window;
    sf::RectangleShape sidebar_rect;
    sf::Font font;
    sf::Text input;
    sf::Text raddeg_mode;
    sf::Text rgb_mode;
    sf::Text polar_mode;
    List<sf::Text> previous_inputs;

    // Input handing and animation
    string last_input;
    string current_input;
    int typing_animation_frame;
    bool typing;

    // Mouse input handling 
    int cursor_pos;
    bool left_mouse_down;
    sf::Vector2i mouse_last_pos;
    clock_t timer;

    // UI scaling/dpmain variables
    bool recently_resized;
    float relative_unit_x;
    float relative_unit_y;
    double graph_scale;
    int font_size;
    sf::Vector2f center;

    // Run functions
    void process_events();
    void update();

    // UI managers
    void manage_current_input();
    void manage_previous_inputs();
    void manage_toggles();
    void frame_text_sidebar(sf::Text& object, string text);
};

#endif