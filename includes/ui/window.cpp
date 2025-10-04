#include "window.h"

Window::Window(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT, unsigned int FRAME_RATE):
    system(),

    window(),
    sidebar_rect(),
    font("NotoSans.ttf"),
    input(font, "", font_size),
    raddeg_mode(font, "RAD", font_size),
    rgb_mode(font, "RGB", font_size),
    polar_mode(font, "CRTES", font_size),
    previous_inputs(),

    last_input(""),
    current_input(""),
    typing_animation_frame(0),
    typing(false),
    
    cursor_pos(0),
    left_mouse_down(false),
    timer(clock()),
    
    recently_resized(false),
    relative_unit_x(1),
    relative_unit_y(1),
    graph_scale(1),
    font_size(21),
    center((WINDOW_WIDTH/2) + ((WINDOW_WIDTH/5)/2), WINDOW_HEIGHT/2)
{
    window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Graphing Calculator");
    window.setFramerateLimit(FRAME_RATE);

    auto image = sf::Image{};
    if (image.loadFromFile("icon.png"))
    {
        window.setIcon(image.getSize(), image.getPixelsPtr());
    }

    system.set_center(center);
    system.input(current_input);
    system.eval(window.getSize());

    ifstream hist_file("history.txt");
    string line = "";
    if (hist_file.is_open()) {
        while (getline(hist_file, line)) {
            previous_inputs.insert_after(sf::Text(font, line, 0), previous_inputs.last_node());
        }
        hist_file.close();
    }
}

/*
    Runs the window
*/
void Window::run() {
    while (window.isOpen()) {
        process_events();
        update();
    }
}

/*
    Processes window events
*/
void Window::process_events() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            if (system.valid_equation())
                previous_inputs.insert_after(sf::Text(font, last_input), previous_inputs.last_node());

            while (previous_inputs.size() > 10)
                previous_inputs.Delete(previous_inputs.begin());
            
            ofstream hist_file("history.txt");
            for (List<sf::Text>::Iterator it = previous_inputs.begin(); it != previous_inputs.end(); it++) {
                hist_file << (string)it->getString() << endl;
            }
            hist_file.close();
            window.close();
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            const sf::Event::MouseButtonPressed* mbp_event = event->getIf<sf::Event::MouseButtonPressed>();

            if (mbp_event->button != sf::Mouse::Button::Left) 
                continue;

            sf::Vector2f pos = sf::Vector2f(mbp_event->position.x, mbp_event->position.y);
            typing = false;

            // See if click was in the sidebar region
            if (sidebar_rect.getGlobalBounds().contains(pos)) {
                // See if input box was clicked
                if (0 <= pos.y && relative_unit_y*50 >= pos.y) {
                    typing =     true;
                    cursor_pos = current_input.size();
                }

                // See if previous inputs were clicked
                for (List<sf::Text>::Iterator it = previous_inputs.begin(); it != previous_inputs.end(); it++) {
                    if (it->getGlobalBounds().position.y <= pos.y && it->getGlobalBounds().position.y + it->getGlobalBounds().size.y >= pos.y) {
                        current_input = it->getString();
                        if (system.valid_equation() && current_input != last_input)
                            previous_inputs.insert_after(sf::Text(font, last_input), previous_inputs.last_node());
                        previous_inputs.Delete(it);
                        system.input(current_input);
                        system.eval(window.getSize());
                        last_input = current_input;
                        break;
                    }
                }

                // See if graph toggles were clicked
                if (rgb_mode.getGlobalBounds().contains(pos)) {
                    system.toggle_rgb();
                    system.eval(window.getSize());
                }
                else if (raddeg_mode.getGlobalBounds().contains(pos)) {
                    system.toggle_rad();
                    system.eval(window.getSize());
                }
                else if (polar_mode.getGlobalBounds().contains(pos)) {
                    system.toggle_polar();
                    system.eval(window.getSize());
                }
            }
            else {
                // Check for double click to reset graph view
                double duration = static_cast<double>(clock() - timer) / CLOCKS_PER_SEC;
                timer = clock();
                if (duration < 0.25) {
                    graph_scale      = 1;
                    center           = sf::Vector2f((window.getSize().x/2) + ((window.getSize().x/5)/2), window.getSize().y/2);
                    recently_resized = true;
                    relative_unit_x  = 1 * ((float)window.getSize().x/1260);
                    relative_unit_y  = 1 * ((float)window.getSize().y/720);
                    font_size        = relative_unit_x * 21;
                    
                    system.set_scale(graph_scale);
                    system.set_center(center);
                    system.eval(window.getSize());
                }
                // Otherwise do graph pan
                else {
                    left_mouse_down = true;
                    mouse_last_pos  = mbp_event->position;

                    if (system.valid_equation() && current_input != last_input)
                        previous_inputs.insert_after(sf::Text(font, last_input), previous_inputs.last_node());
                    system.input(current_input);
                    system.eval(window.getSize());
                    last_input = current_input;  
                }
            }
        }

        if (event->is<sf::Event::MouseButtonReleased>()) {
            const sf::Event::MouseButtonReleased* mbr_event = event->getIf<sf::Event::MouseButtonReleased>();
            if (mbr_event->button == sf::Mouse::Button::Left) {
                left_mouse_down = false;
            }
        }

        if (event->is<sf::Event::MouseMoved>()) {
            const sf::Event::MouseMoved* mm_event = event->getIf<sf::Event::MouseMoved>();
            if (recently_resized) {
                mouse_last_pos    = mm_event->position;
                recently_resized = false;
            }
            else if (left_mouse_down) {
                sf::Vector2i delta(mm_event->position.x - mouse_last_pos.x, mm_event->position.y - mouse_last_pos.y);
                
                float x = center.x + delta.x;
                float y = center.y + delta.y;
                if (x > X_MAX)
                    x = X_MAX;
                else if (x < X_MIN)
                    x = X_MIN;
                if (y > Y_MAX)
                    y = Y_MAX;
                else if (y < Y_MIN)
                    y = Y_MIN;

                center = sf::Vector2f(x, y);
                mouse_last_pos = mm_event->position;
                
                system.set_center(center);
                system.eval(window.getSize());
            }
        }

        if (event->is<sf::Event::KeyPressed>()) {
            const sf::Event::KeyPressed* kp_event = event->getIf<sf::Event::KeyPressed>();

            if (!typing) 
                continue;

            if (kp_event->scancode == sf::Keyboard::Scancode::Left) {
                cursor_pos--;
                if (cursor_pos < 0)
                    cursor_pos = 0;
            }
            else if (kp_event->scancode == sf::Keyboard::Scancode::Right) {
                cursor_pos++;
                if (cursor_pos > current_input.size())
                    cursor_pos = current_input.size();
            }
            else if (kp_event->scancode == sf::Keyboard::Scancode::Backspace) {
                if (current_input.size() == 0 || cursor_pos == 0)
                    continue;
                
                string temp = current_input.substr(0, cursor_pos-1) + current_input.substr(cursor_pos, current_input.size());
                cursor_pos--;
                if (cursor_pos < 0)
                    cursor_pos = 0;
                current_input = temp;
            }
            else if (kp_event->scancode == sf::Keyboard::Scancode::Enter) {
                if (system.valid_equation() && current_input != last_input)
                    previous_inputs.insert_after(sf::Text(font, last_input), previous_inputs.last_node());
                typing     = false;
                last_input = current_input;   

                system.input(current_input);
                system.eval(window.getSize());
            }
        }
        else if (event->is<sf::Event::TextEntered>()) {
            const sf::Event::TextEntered* te_event = event->getIf<sf::Event::TextEntered>();

            if (typing && te_event->unicode != 8) {
                string temp = current_input.substr(0, cursor_pos) + static_cast<char>(te_event->unicode) + current_input.substr(cursor_pos, current_input.size());
                cursor_pos++;
                current_input = temp;
            }
        }

        if (event->is<sf::Event::Resized>()) {
            const sf::Event::Resized* r_event = event->getIf<sf::Event::Resized>();

            graph_scale      = 1;
            recently_resized = true;
            relative_unit_x  = 1 * ((float)r_event->size.x/1260);
            relative_unit_y  = 1 * ((float)r_event->size.y/720);
            font_size        = relative_unit_x * 21;

            sf::FloatRect view(sf::Vector2f(0, 0), sf::Vector2f(r_event->size.x, r_event->size.y));
            window.setView(sf::View(view));

            system.set_scale(graph_scale);
            system.eval(window.getSize());
        }

        if (event->is<sf::Event::MouseWheelScrolled>()) {
            const sf::Event::MouseWheelScrolled* scr_event = event->getIf<sf::Event::MouseWheelScrolled>();
            
            if ((graph_scale - (0.075 * (scr_event->delta * graph_scale))) < SCALE_MIN || (graph_scale - (0.075 * (scr_event->delta * graph_scale))) > SCALE_MAX)
                continue;
            
            graph_scale = graph_scale - (0.075 * (scr_event->delta * graph_scale));
            
            system.set_scale(graph_scale);
            system.eval(window.getSize());
        }
    }
}

/*
    Clears the windows, handles updates, and renders
*/
void Window::update() {
    window.clear();

    // Draw graph & grid
    system.draw(window);

    // Draw sidebar
    sidebar_rect.setFillColor(sf::Color(15, 15, 15));
    sidebar_rect.setSize(sf::Vector2f(relative_unit_x*252, window.getSize().y));
    sidebar_rect.setPosition(sf::Vector2f(0, 0));
    window.draw(sidebar_rect);

    // Manage input boxes and toggles
    manage_previous_inputs();
    manage_current_input();
    manage_toggles();

    window.display();
}

/*
    Manages previous inputs and lists them out on the sidebar
*/
void Window::manage_previous_inputs() {
    while (previous_inputs.size() > 10)
        previous_inputs.Delete(previous_inputs.begin());

    int count = 0;
    int size = previous_inputs.size();
    float space = relative_unit_y*60;
    for (List<sf::Text>::Iterator it = previous_inputs.begin(); it != previous_inputs.end(); it++) {
        it->setCharacterSize(font_size);
        it->setFillColor(sf::Color(155, 155, 155));
        it->setPosition(sf::Vector2f(relative_unit_x * 10, space * (size-count)));
        frame_text_sidebar(*it, it->getString());
        count++;
    }
}

/*
    Manages UI toggles for the graph
*/
void Window::manage_toggles() {
    float space = relative_unit_y*60;
    
    // Radian/degree toggle
    raddeg_mode.setString((system.rad_mode()) ? "RAD" : "DEG");
    raddeg_mode.setPosition(sf::Vector2f(relative_unit_x * 31, space * 11));
    raddeg_mode.setCharacterSize(font_size);
    window.draw(raddeg_mode);

    // Polar/cartesian toggle
    polar_mode.setString((system.polar_mode()) ? "POLAR" : "CRTES");
    polar_mode.setPosition(sf::Vector2f(relative_unit_x * 93, space * 11));
    polar_mode.setCharacterSize(font_size);
    window.draw(polar_mode);

    // RGB toggle
    rgb_mode.setPosition(sf::Vector2f(relative_unit_x * 178, space * 11));
    rgb_mode.setCharacterSize(font_size);
    if (system.rgb_mode())
        rgb_mode.setFillColor(system.get_rainbow());
    else   
        rgb_mode.setFillColor(sf::Color(255, 255, 255));
    window.draw(rgb_mode);
}

/*
    Manages current input UI
*/
void Window::manage_current_input() {
    input.setCharacterSize(relative_unit_x*21);
    input.setPosition(sf::Vector2f(relative_unit_x*10, relative_unit_y*10));

    sf::RectangleShape background;
    background.setFillColor(sf::Color(35, 35, 35));
    background.setSize(sf::Vector2f(relative_unit_x*252, relative_unit_y*50));

    if (typing) {
        float x_size = (relative_unit_x*252 > input.getGlobalBounds().size.x+20) ? relative_unit_x*252 : input.getGlobalBounds().size.x+20;
        background.setSize(sf::Vector2f(x_size, relative_unit_y*50));
        window.draw(background);
        
        typing_animation_frame++;
        if (typing_animation_frame > 30)
            typing_animation_frame = 0;

        if (typing_animation_frame <= 15)
            input.setString(current_input.substr(0, cursor_pos) + "|" + current_input.substr(cursor_pos, current_input.size()));
        else 
            input.setString(current_input.substr(0, cursor_pos) + " " + current_input.substr(cursor_pos, current_input.size()));

        window.draw(input);
    }
    else {
        if (!system.valid_equation() && current_input.size() != 0)
            background.setFillColor(sf::Color(125, 15, 15));
        window.draw(background);
        frame_text_sidebar(input, current_input);
    }
}

/*
    Frames `sf::Text` objects so that they don't go over the sidebar
*/
void Window::frame_text_sidebar(sf::Text& object, string text) {
    sf::Text stunt_double = object;
    object.setString(text);

    int rem_char = 0;
    while (stunt_double.getGlobalBounds().size.x > sidebar_rect.getGlobalBounds().size.x - 20) {
        rem_char++;
        stunt_double.setString(text.substr(0, text.size()-rem_char));
    }
    if (rem_char != 0) {
        rem_char += 2;
        stunt_double.setString(text.substr(0, text.size()-rem_char) + "..");
        window.draw(stunt_double);
    }
    else {
        window.draw(stunt_double);
    }
}
