//
// Created by vcherkov on 08.05.17.
//

#include "exit_program.h"
#include "game_ower.h"
#include "play.h"
#include "menu.h"

game_over* game_over::instance(game_engine *engine) {
    static game_over _game_over(engine);
    return &_game_over;
}

void game_over::handle_events(game_engine *engine) {
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

void game_over::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
}

void game_over::draw() {
    main_grid->draw();
}

game_over::game_over(game_engine *engine) :
        mouse_pressed(false),
        main_grid(nullptr),
        pressed(nullptr)
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> game_over::create_main_grid(game_engine *engine) {
    global_parameters::bounds btn_bounds =
            global_parameters::get_controller_bounds("button", "usual");

    button_t btns[3] = {button_t::RESTART, button_t::EXIT};

    shared_ptr <grid> main_grid = make_shared <grid> (engine->get_window(),
                                                      btn_bounds.size.first * 2,
                                                      btn_bounds.size.second * 2);

    shared_ptr <i_controller> frm =
            make_shared <frame> (engine->get_window(),
                                btn_bounds.size.first * 2,
                                btn_bounds.size.second, "GAME OVER");
    engine->get_designer()->wear_controller(frm);
    main_grid->set_controller(frm, {0, 0});

    for(uint i = 0; i < 2; i++) {
        shared_ptr <i_controller> btn =
                make_shared <button> (engine->get_window(), btns[i]);
        engine->get_designer()->wear_controller(btn);
        main_grid->set_controller(btn, {i * btn_bounds.size.first, btn_bounds.size.second});
    }

    main_grid->set_position(sf::Vector2f(
            (engine->get_window()->getSize().x -
             main_grid->get_size().x) / 2.0,
            (engine->get_window()->getSize().y -
             main_grid->get_size().y) / 2.0
    ));

    return main_grid;
}

void game_over::mouse_pressed_action(game_engine *engine) {
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

void game_over::process_button_click(game_engine *engine) {

    if(pressed->get_type() != controller_t::BUTTON)
        return;

    button_t button_type =
            std::static_pointer_cast <button> (pressed)->get_button_t();

    pressed = nullptr;
    mouse_pressed = false;

    switch (button_type) {
        case button_t::EXIT :
            engine->clear_state();
            engine->push_state(menu::instance(engine));
            return;
        case button_t::RESTART :
            engine->clear_state();
            engine->push_state(play::instance(engine));
            return;
        default:
            return;
    }
}

void game_over::mouse_released_action() {
    main_grid->mouse_released_action();
    pressed = nullptr;
}