#include "system.h"

System::System():
    x_scale(50), 
    y_scale(50), 
    scale(1), 

    shunting_yard(),
    rpn(),
    valid(false), 
    graph_center(sf::Vector2f(0, 0)), 

    rgb(false),
    rainbow(sf::Color(255, 0, 0)),
    r(false),
    g(true),

    polar(false),

    font("NotoSans.ttf"),
    step(1), 
    scale_back(0),
    scale_next(0)
{
    // Empty
}

/*
    Takes in a new equation and sets it up for graphing evaluation
    @param new_eq String of the new equation
*/
void System::input(string new_eq) {
    valid = true;
    points.clear();

    if (new_eq.empty()) {
        rpn = RPN();
        shunting_yard = ShuntingYard();
        valid = false;
        return;
    }

    if (shunting_yard.infix_string(new_eq)) {
        rpn.set_input(shunting_yard.postfix());
    } 
    else {
        valid = false; 
    }
}

/*
    Evaluates the current input based on the current window size and graph center
    @param window_size Size of the current window
*/
void System::eval(sf::Vector2u window_size) {
    if (!valid)   
        return;

    points.clear();

    try {
        float y,
              last_point_y = 0,
              step_window  = 0.5;

        for (float i = 0; i <= window_size.x; i+=step_window) {
            y = rpn(get_x_relative(i));

            points.push_back(sf::Vector2f(get_x_relative(i), y));

            step_window = 1/(abs(y-last_point_y));
            if (step_window < 0.001)
                step_window = 0.001;
            if (step_window > 0.5 || y == INFINITY || last_point_y == INFINITY)
                step_window = 0.5;
            if (isnan(step_window))
                step_window = 0.5;

            last_point_y = y;
        }
    } catch(int e) {
        valid = false;
    }
}

/*
    Sets the scale of the graph
    @param new_scale Scale of the graph (lower is zoomed in, higher is zoomed out)
*/
void System::set_scale(double new_scale) {
    scale   = new_scale;
    x_scale = 50 / scale;
    y_scale = 50 / scale;
}

/*
    Sets the new center of the graph
    @param center Center of the graph as a `sf::Vector2f`
*/
void System::set_center(sf::Vector2f center) {
    graph_center = center;
}

/*
    Returns whether the current input is a valid equation
    @return `true` if valid, `false` if not
*/
bool System::valid_equation() {
    return valid;
}

/* 
    Converts graph X coordinates into screen resolution X coordinates 
    @param x_relative X coordinate relative to the graph
    @return X coordinate relative to the screen
*/
float System::get_x_actual(float x_relative) {
    return graph_center.x + (x_relative * x_scale);
}

/* 
    Converts graph Y coordinates into screen resolution Y coordinates 
    @param y_relative Y coordinate relative to the graph
    @return Y coordinate relative to the screen
*/
float System::get_y_actual(float y_relative) {
    return graph_center.y - (y_relative * y_scale);
}

/* 
    Converts screen resolution X coordinates to graph X coordinates
    @param x_actual X coordinate relative to the screen
    @return X coordinate relative to the graph
*/
float System::get_x_relative(float x_actual) {
    return (x_actual - graph_center.x)/x_scale;
}

/* 
    Converts screen resolution Y coordinates to graph Y coordinates
    @param y_actual Y coordinate relative to the screen
    @return Y coordinate relative to the graph
*/
float System::get_y_relative(float y_actual) {
    return (graph_center.y - y_actual)/y_scale;
}

/*
    Progresses the rainbow to the next color
*/
void System::step_rainbow() {
    if (r) {
        if (rainbow.b > 0)
            rainbow.b-=3;
        rainbow.r+=3;
        if (rainbow.r >= 255) {
            r = false;
            g = true;
        }
    }
    else if (g) {
        if (rainbow.r > 0)
            rainbow.r-=3;
        rainbow.g+=3;
        if (rainbow.g >= 255) {
            g = false;
        }
    }
    else {
        if (rainbow.g > 0)
            rainbow.g-=3;
        rainbow.b+=3;
        if (rainbow.b >= 255) {
            r = true;
        }
    }
}

/*
    Draws the graph and grid to the provided window
    @param window `sf::RenderWindow` to draw to
*/
void System::draw(sf::RenderWindow& window) {
    step_rainbow();
    
    if (polar)
        generate_polar_grid(window);
    else
        generate_grid(window);

    if (!valid) 
        return; 

    auto prev = points.begin();
    for (auto it = prev; it != points.end(); it++) {
        if (it->y != INFINITY && abs(get_y_actual(prev->y) - get_y_actual(it->y)) < 2) {
            sf::CircleShape circle;
            circle.setRadius(1);
            circle.setFillColor(sf::Color(255, 255, 255));
            if (rgb)
                circle.setFillColor(rainbow);
            circle.setOrigin(sf::Vector2f(circle.getGlobalBounds().getCenter().x, circle.getGlobalBounds().getCenter().y));
            circle.setPosition(sf::Vector2f(get_x_actual(it->x), get_y_actual(it->y)));
            window.draw(circle);
        }
        else if ((prev->y != INFINITY && it->y != INFINITY) && ((abs(get_y_actual(it->y) - get_y_actual(prev->y))) < window.getSize().y * y_scale)) {
            sf::ConvexShape line;
            line.setPointCount(4);
            line.setPoint(0, sf::Vector2f(get_x_actual(prev->x)-(1), get_y_actual(prev->y)));
            line.setPoint(1, sf::Vector2f(get_x_actual(it->x)-(1), get_y_actual(it->y)));        
            line.setPoint(2, sf::Vector2f(get_x_actual(it->x)+(1), get_y_actual(it->y)));
            line.setPoint(3, sf::Vector2f(get_x_actual(prev->x)+(1), get_y_actual(prev->y)));
            line.setFillColor(sf::Color(255, 255, 255));
            if (rgb)
                line.setFillColor(rainbow);
            window.draw(line);
        }

        prev = it;
    }
}

/*
    Handles positioning for axis labels for the graph's grid
    @param text `sf::Text` to use as label
    @param window_size Size of the `sf::RenderWindow`
    @param num X/Y coordinate
    @param x_label `true` if the label is for the X-axis, `false` if for the Y-axis
*/
void System::set_grid_label(sf::Text& text, sf::Vector2u window_size, int num, bool x_label) { 
    text.setOrigin(sf::Vector2f(text.getGlobalBounds().getCenter().x, text.getGlobalBounds().getCenter().y));

    if (x_label) {
        float y_pos = get_y_actual(0);
        if (y_pos + 20 > window_size.y)
            y_pos = (window_size.y) - (20);
        else if (y_pos - 20 < 0)
            y_pos = 20;
        text.setPosition(sf::Vector2f(get_x_actual(num), y_pos));
    }
    else {
        float x_pos = get_x_actual(0);
        if (x_pos + 20 > window_size.x)
            x_pos = window_size.x - (20);
        else if (x_pos - 20 < window_size.x/5)
            x_pos = window_size.x/5 + (20);
        text.setPosition(sf::Vector2f(x_pos, get_y_actual(num)));
    }
}

/*
    Generates the graph's grid
    @param window `sf::RenderWindow` to draw the graph to
*/
void System::generate_grid(sf::RenderWindow& window) {
    step = (get_x_relative(window.getSize().x) - get_x_relative(0))/10;
    if (step < 1)
        step = 1;
    if (step > 5)
        step -= step % 5;
    
    // Draw axies

    for (int i = get_x_relative(0); i <= get_x_relative(window.getSize().x); i++) {
        if (i % step != 0) 
            continue;

        sf::RectangleShape vertical_axis(sf::Vector2f(3, window.getSize().y));
        if (i == 0)
            vertical_axis.setSize(sf::Vector2f(5, window.getSize().y));
        vertical_axis.setFillColor(sf::Color(25, 25, 25));
        vertical_axis.setOrigin(sf::Vector2f(vertical_axis.getGlobalBounds().getCenter().x, 0));
        vertical_axis.setPosition(sf::Vector2f(get_x_actual(i), 0));
        window.draw(vertical_axis);
    }

    for (int i = get_y_relative(0); i >= get_y_relative(window.getSize().y); i--) {
        if (i % step != 0) 
            continue;

        sf::RectangleShape horizontal_axis(sf::Vector2f(window.getSize().x, 3));
        if (i == 0)
            horizontal_axis.setSize(sf::Vector2f(window.getSize().x, 5));
        horizontal_axis.setFillColor(sf::Color(25, 25, 25));
        horizontal_axis.setOrigin(sf::Vector2f(0, horizontal_axis.getGlobalBounds().getCenter().y));
        horizontal_axis.setPosition(sf::Vector2f(0, get_y_actual(i)));
        window.draw(horizontal_axis);
    }

    // Draw labels

    for (int i = get_x_relative(0); i <= get_x_relative(window.getSize().x); i++) {
        if (i % step != 0) 
            continue;

        sf::Text vertical_label(font, to_string(i), 20U);
        set_grid_label(vertical_label, window.getSize(), i, true);
        vertical_label.setFillColor(sf::Color(150, 150, 150));
        window.draw(vertical_label);
    }

    for (int i = get_y_relative(0); i >= get_y_relative(window.getSize().y); i--) {
        if (i % step != 0 || i == 0) 
            continue;

        sf::Text horizontal_label(font, to_string(i), 20U);
        set_grid_label(horizontal_label, window.getSize(), i, false);
        horizontal_label.setFillColor(sf::Color(150, 150, 150));
        window.draw(horizontal_label);
    }
}

/*
    Generates the graph's polar grid
    @param window `sf::RenderWindow` to draw the graph to
*/
void System::generate_polar_grid(sf::RenderWindow& window) {
    if (get_x_relative(0) <= 0 && get_x_relative(window.getSize().x) >= 0) {
        sf::RectangleShape vertical_axis(sf::Vector2f(5, window.getSize().y));
        vertical_axis.setFillColor(sf::Color(25, 25, 25));
        vertical_axis.setOrigin(sf::Vector2f(vertical_axis.getGlobalBounds().getCenter().x, 0));
        vertical_axis.setPosition(sf::Vector2f(get_x_actual(0), 0));
        window.draw(vertical_axis);
    }

    if (get_y_relative(0) >= 0 && get_y_relative(window.getSize().y) <= 0) {
        sf::RectangleShape horizontal_axis(sf::Vector2f(window.getSize().x, 5));
        horizontal_axis.setFillColor(sf::Color(25, 25, 25));
        horizontal_axis.setOrigin(sf::Vector2f(0, horizontal_axis.getGlobalBounds().getCenter().y));
        horizontal_axis.setPosition(sf::Vector2f(0, get_y_actual(0)));
        window.draw(horizontal_axis);
    }

    step = (get_x_relative(window.getSize().x) - get_x_relative(0))/10;
    if (step < 1)
        step = 1;
    if (step > 5)
        step -= step % 5;
    
    float x_start     = get_x_relative(0),
          x_end       = get_x_relative(window.getSize().x),
          y_start     = get_y_relative(0),
          y_end       = get_y_relative(window.getSize().y),
          diag_pospos = sqrt(pow(get_x_relative(window.getSize().x), 2) + pow(get_y_relative(0), 2)),
          diag_negpos = sqrt(pow(get_x_relative(0), 2) + pow(get_y_relative(0), 2)),
          diag_negneg = sqrt(pow(get_x_relative(0), 2) + pow(get_y_relative(window.getSize().y), 2)),
          diag_posneg = sqrt(pow(get_x_relative(window.getSize().x), 2) + pow(get_y_relative(window.getSize().y), 2)),
          r_start     = min({x_start, x_end, y_start, y_end, diag_pospos, diag_negpos, diag_negneg, diag_posneg}),
          r_end       = max({x_start, x_end, y_start, y_end, diag_pospos, diag_negpos, diag_negneg, diag_posneg});

    for (int i = r_start; i <= r_end; i++) {
        if (i % step != 0 || i == 0) 
            continue;

        sf::CircleShape circle_axis;
        circle_axis.setRadius(abs(i)*x_scale);
        circle_axis.setOrigin(sf::Vector2f(circle_axis.getGlobalBounds().getCenter().x, circle_axis.getGlobalBounds().getCenter().y));
        circle_axis.setPosition(sf::Vector2f(get_x_actual(0), get_y_actual(0)));
        circle_axis.setFillColor(sf::Color::Transparent);
        circle_axis.setOutlineColor(sf::Color(25, 25, 25));
        circle_axis.setOutlineThickness(3);
        window.draw(circle_axis);
    }

    for (int i = 0; i <= 360; i += 15) {
        if (i % 90 == 0)
            continue;

        sf::RectangleShape angular_line;
        angular_line.setSize(sf::Vector2f(3, pow(get_x_actual(r_end), 2)));
        angular_line.setPosition(sf::Vector2f(get_x_actual(0), get_y_actual(0)));
        angular_line.setFillColor(sf::Color(25, 25, 25));
        angular_line.setRotation(sf::degrees(i));
        window.draw(angular_line);
    }

    for (int i = get_x_relative(0); i <= get_x_relative(window.getSize().x); i++) {
        if (i % step != 0 || i == 0) 
            continue;

        sf::Text vertical_label(font, to_string(abs(i)), 20U);
        vertical_label.setOrigin(sf::Vector2f(vertical_label.getGlobalBounds().getCenter().x, vertical_label.getGlobalBounds().getCenter().y));
        vertical_label.setPosition(sf::Vector2f(get_x_actual(i), get_y_actual(0)));
        vertical_label.setFillColor(sf::Color(150, 150, 150));
        window.draw(vertical_label);
    }

    for (int i = get_y_relative(0); i >= get_y_relative(window.getSize().y); i--) {
        if (i % step != 0 || i == 0) 
            continue;

        sf::Text horizontal_label(font, to_string(abs(i)), 20U);
        horizontal_label.setOrigin(sf::Vector2f(horizontal_label.getGlobalBounds().getCenter().x, horizontal_label.getGlobalBounds().getCenter().y));
        horizontal_label.setPosition(sf::Vector2f(get_x_actual(0), get_y_actual(i)));
        horizontal_label.setFillColor(sf::Color(150, 150, 150));
        window.draw(horizontal_label);
    }
}

/*
    `sf::Color` that changes through different colors to create a rainbow effect
    @return Progressively changing `sf::Color` 
*/
sf::Color System::get_rainbow() {
    return rainbow;
}

/*
    Returns whether the graph is in radians or degrees
    @return `true` if in radians, `false` if in degrees
*/
bool System::rad_mode(){
    return (rpn.get_mode() == RADIAN);
}

/*
    Returns whether the graph is in RGB mode
    @return `true` or `false`
*/
bool System::rgb_mode(){
    return rgb;
}

/*
    Returns whether the graph is in cartesian or polar
    @return `true` if in polar, `false` if in cartesian
*/
bool System::polar_mode() {
    return polar;
}

/*
    Toggles between radians/degrees
*/
void System::toggle_rad() {
    if (rpn.get_mode() == DEGREE)
        rpn.set_mode(RADIAN);
    else    
        rpn.set_mode(DEGREE);
}

/*
    Toggles between RGB mode on/off
*/
void System::toggle_rgb() {
    rgb = !rgb;
}

/*
    Toggles between polar/cartesian mode
*/
void System::toggle_polar() {
    polar = !polar;
}
