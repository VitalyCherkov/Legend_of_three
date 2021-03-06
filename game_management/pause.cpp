//
// Created by vcherkov on 08.05.17.
//

#include "exit_program.h"
#include "play.h"
#include "menu.h"
#include "pause.h"

pause* pause::instance(game_engine *engine) {
    static pause _pause(engine);
    return &_pause;
}

void pause::handle_events(game_engine *engine) {
    sf::Event event;
    while (engine->get_window()->pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::Closed :
                engine->push_state(exit_program::instance(engine));
                mouse_pressed = false;
                break;
            case sf::Event::MouseButtonPressed :
                mouse_pressed = true;
                break;
            case sf::Event::MouseButtonReleased :
                mouse_released_action();
                mouse_pressed = false;
                break;
            default:
                break;
        }
        if(mouse_pressed)
            mouse_pressed_action(engine);
    }
}

void pause::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
}

void pause::draw() {
    main_grid->draw();
}

pause::pause(game_engine *engine) :
        mouse_pressed(false),
        main_grid(nullptr),
        pressed(nullptr)
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> pause::create_main_grid(game_engine *engine) {
    if(!engine)
        return nullptr;

    global_parameters::bounds btn_bounds =
            global_parameters::get_controller_bounds("button", "usual");

    button_t btns[3] = {button_t::RESTART, button_t::PLAY, button_t::EXIT};

    shared_ptr <grid> main_grid = make_shared <grid> (engine->get_window(),
                                                      btn_bounds.size.first * 3,
                                                      btn_bounds.size.second);
    for(uint i = 0; i < 3; i++) {
        shared_ptr <i_controller> btn =
                make_shared <button> (engine->get_window(), btns[i]);
        engine->get_designer()->wear_controller(btn);
        main_grid->set_controller(btn, {i * btn_bounds.size.first, 0});
    }

    main_grid->set_position(sf::Vector2f(
            (engine->get_window()->getSize().x -
             main_grid->get_size().x) / 2.0,
            (engine->get_window()->getSize().y -
             main_grid->get_size().y) / 2.0
    ));

    return main_grid;
}

void pause::mouse_pressed_action(game_engine *engine) {
    if(!main_grid->is_focus_on())
        return;

    if(!pressed)
        pressed = std::static_pointer_cast <grid>
                (main_grid)->get_controller_in_focus();

    if(!pressed)
        return;

    pressed->mouse_pressed_action();
    process_button_click(engine);
}

void pause::process_button_click(game_engine *engine) {
    button_t button_type =
            std::static_pointer_cast <button> (pressed)->get_button_t();

    pressed = nullptr;
    mouse_pressed = false;

    switch (button_type) {
        case button_t::EXIT :
            engine->clear_state();
            engine->push_state(menu::instance(engine));
            return;
        case button_t::PLAY :
            engine->pop_state();
            return;
        case button_t::RESTART :
            engine->clear_state();
            engine->push_state(play::instance(engine));
            return;
        default:
            return;
    }
}

void pause::mouse_released_action() {
    main_grid->mouse_released_action();
    pressed = nullptr;
}