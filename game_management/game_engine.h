//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_GAME_ENGINE_H
#define LEGEND_OF_THREE_GAME_ENGINE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "../level.h"
#include "../designer_t.h"

using std::vector;
using std::string;
using std::shared_ptr;

class i_state;

class game_engine {
public:
    game_engine(const string &title = "",
                size_t width = 0, size_t height = 0);

    void clear();

    void push_state(i_state* state);
    void pop_state();
    void clear_state();

    void handle_events();
    void update();
    void draw();

    bool running();
    void quit();

    const level &get_level() const;
    const shared_ptr <sf::RenderWindow> &get_window() const;
    const shared_ptr <designer_t> &get_designer() const;
    void load_new_level();
    void load_level(uint index);
    float get_time() const;

private:
    shared_ptr <sf::RenderWindow> window;
    shared_ptr <designer_t> designer;
    sf::Clock clock;

    vector <i_state*> states;

    level lvl;

    int cur_level;
    bool _running;
    bool fullscreen;
};


#endif //LEGEND_OF_THREE_GAME_ENGINE_H
