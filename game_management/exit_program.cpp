//
// Created by vcherkov on 08.05.17.
//

#include "exit_program.h"

exit_program* exit_program::instance(game_engine *engine) {
    static exit_program _exit(engine);
    return &_exit;
}

void exit_program::handle_events(game_engine *engine) {
    sf::Event event;
    while (engine->get_window()->pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::Closed :
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

void exit_program::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
}

void exit_program::draw() {
    main_grid->draw();
}

exit_program::exit_program(game_engine *engine) :
    mouse_pressed(false),
    pressed(nullptr),
    main_grid(nullptr)
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> exit_program::create_main_grid(game_engine *engine) {
    if(!engine)
        return nullptr;

    global_parameters::bounds btn_bounds =
            global_parameters::get_controller_bounds("button", "usual");

    shared_ptr <grid> main_grid =
            make_shared <grid> (engine->get_window(),
                                btn_bounds.size.first * 2,
                                btn_bounds.size.second * 2);


    shared_ptr <i_controller> question =
            make_shared <frame> (engine->get_window(),
                                 btn_bounds.size.first * 2,
                                 btn_bounds.size.second, "Are you sure?");
    engine->get_designer()->wear_controller(question);

    shared_ptr <i_controller> yes_b =
            make_shared <button> (engine->get_window(),
                                  button_t::USUAL, "Yes");
    engine->get_designer()->wear_controller(yes_b);

    shared_ptr <i_controller> no_b =
            make_shared <button> (engine->get_window(),
                                  button_t::USUAL, "No");
    engine->get_designer()->wear_controller(no_b);

    main_grid->set_controller(question, {0, 0});
    main_grid->set_controller(no_b, {0, btn_bounds.size.second});
    main_grid->set_controller(yes_b, {btn_bounds.size.first,
                                     btn_bounds.size.second});

    main_grid->set_position(sf::Vector2f(
            (engine->get_window()->getSize().x -
                    main_grid->get_size().x) / 2.0,
            (engine->get_window()->getSize().y -
                    main_grid->get_size().y) / 2.0
    ));

    return main_grid;
}

void exit_program::mouse_pressed_action(game_engine *engine) {
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

void exit_program::process_button_click(game_engine *engine) {
    if(pressed->get_type() == controller_t::BUTTON) {
        sf::String button_name =
                std::static_pointer_cast <button> (pressed)->get_label();

        if(button_name == "No") {
            pressed = nullptr;
            mouse_pressed = false;
            engine->pop_state();
        }
        else
            engine->quit();
    }
}

void exit_program::mouse_released_action() {
    main_grid->mouse_released_action();
    pressed = nullptr;
}