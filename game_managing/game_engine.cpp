//
// Created by vcherkov on 08.05.17.
//

#include "game_engine.h"
#include "menu.h"
#include "exit_program.h"
#include "play.h"

game_engine::game_engine(const string &title,
                         size_t width, size_t height) :
        window(std::make_shared <sf::RenderWindow>
                       (sf::VideoMode(width, height), title)),
        designer(nullptr),
        clock(),
        states(),
        lvl(),
        cur_level(1),
        fullscreen(false)
{
    global_parameters::load_global_parameters
            ("xml_files/global_parameters.xml");

    designer = std::make_shared <designer_t>
                     (global_parameters::get_field_texture(),
                      global_parameters::get_skin_texture(),
                      global_parameters::get_main_texture());

    lvl.load_from_file(global_parameters::get_level_file(0));

    states.push_back(menu::instance(this));
}

void game_engine::clear() {
    window->close();
}

void game_engine::push_state(i_state* state) {
    states.push_back(state);
    states.back()->init();
}

void game_engine::pop_state() {
    if(!states.empty()) {
        states.back()->clear();
        states.pop_back();
    }
}

void game_engine::clear_state() {
    while(!states.empty())
        pop_state();
}

void game_engine::handle_events() {
    if(!states.empty())
        states.back()->handle_events(this);
}

void game_engine::update() {
    if(!states.empty())
        states.back()->update(this);
    clock.restart();
}

void game_engine::draw() {
    window->clear(sf::Color::White);
    for(size_t i = 0; i < states.size(); i++)
        states[i]->draw();
    window->display();
}

bool game_engine::running() {
    return _running;
}

void game_engine::quit() {
    _running = false;
}

const level& game_engine::get_level() const {
    return lvl;
}

const shared_ptr <sf::RenderWindow> &game_engine::get_window() const {
    return window;
}

const shared_ptr <designer_t> &game_engine::get_designer() const {
    return designer;
}

void game_engine::load_new_level() {
    int new_num = lvl.get_level_number() + 1;
    lvl.load_from_file(global_parameters::get_level_file(new_num));
    play::instance(this)->load_level(this);
}

float game_engine::get_time() const {
    return clock.getElapsedTime().asMilliseconds();
}


