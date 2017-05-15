//
// Created by vcherkov on 08.05.17.
//

#include "exit_program.h"
#include "settings.h"
#include "play.h"
#include "menu.h"


menu* menu::instance(game_engine *engine) {
    static menu _menu(engine);
    return &_menu;
}

void menu::handle_events(game_engine *engine) {
    sf::Event event;
    while (engine->get_window()->pollEvent(event))
    {
        switch (event.type) {
            case sf::Event::Closed :
                engine->push_state(exit_program::instance(engine));
                mouse_pressed = false;
                return;
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

void menu::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
}

void menu::draw() {
    main_grid->draw();
}

menu::menu(game_engine* engine) :
        mouse_pressed(false),
        main_grid(nullptr),
        pressed(nullptr),
        buttons()
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> menu::create_main_grid(game_engine* engine) {
    if(!engine)
        return nullptr;

    vector <string> names = {"Play", "Levels", "Settings", "Help", "Exit"};

    global_parameters::bounds btn_bounds =
            global_parameters::get_controller_bounds("button", "usual_long");

    shared_ptr <grid> main_grid =
            make_shared <grid> (engine->get_window(), btn_bounds.size.first,
                                btn_bounds.size.second * names.size());

    for(uint i = 0; i < names.size(); i++) {
        buttons.push_back(make_shared <button> (engine->get_window(),
                                           button_t::USUAL_long, names[i]));

        engine->get_designer()->wear_controller(buttons[i]);
        main_grid->set_controller(buttons[i], {0,
                                               btn_bounds.size.second * i});
    }

    return main_grid;
}

void menu::mouse_pressed_action(game_engine *engine) {
    if(!main_grid->is_focus_on())
        return;

    if(!pressed)
        pressed = std::static_pointer_cast <grid>
                (main_grid)->get_controller_in_focus();

    process_button_click(engine);
    pressed->mouse_pressed_action();
}

void menu::process_button_click(game_engine *engine) {
    sf::String button_name =
            std::static_pointer_cast <button> (pressed)->get_label();

    if(button_name == "Play") {
        engine->pop_state();
        engine->push_state(play::instance(engine));
    }
    if(button_name == "Settings") {
        engine->push_state(settings::instance(engine));
    }
    if(button_name == "Exit") {
        engine->push_state(exit_program::instance(engine));
    }
}

void menu::mouse_released_action() {
    pressed = nullptr;
}
