#ifndef ALDNWONG_SYSTEM_H
#define ALDNWONG_SYSTEM_H
#include <iostream>
#include <algorithm>
#include "../rpn/rpn.h"
#include "../shunting_yard/shunting_yard.h"
#include <SFML/Graphics.hpp>
#include "constants.h"

using namespace std;

class System {
public:
    // Constructors
    System();

    // Equation evaluation and domain handling
    void input(string new_eq);
    void eval(sf::Vector2u window_size);
    void set_scale(double new_scale);
    void set_center(sf::Vector2f center);
    bool valid_equation();

    // Draw graph
    void draw(sf::RenderWindow& window);

    // Get graph modes
    bool rad_mode();
    bool rgb_mode();
    bool polar_mode();

    // Toggle graph modes
    void toggle_rad();
    void toggle_rgb();
    void toggle_polar();

    // Get rainbow color to sync with sidebar
    sf::Color get_rainbow();

private:
    // Pixels per x/y coordinate
    float x_scale;
    float y_scale;
    float scale;
    float get_x_actual(float x_relative);
    float get_y_actual(float y_relative);
    float get_x_relative(float x_actual);
    float get_y_relative(float y_actual);

    // Equation handling
    ShuntingYard shunting_yard;
    RPN rpn;
    bool valid;
    vector<sf::Vector2f> points;
    sf::Vector2f graph_center;

    // Rainbow colored graph mode
    bool rgb;
    sf::Color rainbow;
    bool r;
    bool g;
    void step_rainbow();

    // Polar graph mode
    bool polar;

    // Grid creation and drawables
    sf::Font font;
    int step;
    int scale_next;
    int scale_back;
    void set_grid_label(sf::Text& text, sf::Vector2u window_size, int num, bool x_label);
    void generate_grid(sf::RenderWindow& window);
    void generate_polar_grid(sf::RenderWindow& window);
};

#endif