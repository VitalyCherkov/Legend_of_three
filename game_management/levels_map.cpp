//
// Created by vcherkov on 08.05.17.
//

#include <sstream>
#include "exit_program.h"
#include "play.h"
#include "levels_map.h"

levels_map* levels_map::instance(game_engine *engine) {
    static levels_map _levels_map(engine);
    return &_levels_map;
}

void levels_map::handle_events(game_engine *engine) {
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

void levels_map::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
}

void levels_map::draw() {
    main_grid->draw();
}

levels_map::levels_map(game_engine *engine) :
        mouse_pressed(false),
        pressed(nullptr),
        main_grid(nullptr)
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> levels_map::create_main_grid
        (game_engine *engine) {

    std::cout << "create\n";

    shared_ptr <grid> main_grid =
            make_shared <grid> (engine->get_window(), 10, 10);

    global_parameters::bounds usual_bnds =
            global_parameters::get_controller_bounds("button", "usual");

    main_grid->set_position(sf::Vector2f(140, 0));

    uint level_number = 1;

    for(uint j = 0; j + usual_bnds.size.second < main_grid->get_rows();
        j += usual_bnds.size.second) {
        bool is_break = false;

        for(uint i = 0; i + usual_bnds.size.first < main_grid->get_cols();
        i+= usual_bnds.size.first) {

            if(level_number > global_parameters::get_num_of_levels()) {
                is_break = true;
                break;
            }

            create_level_button(i, j, level_number, main_grid, engine);
            level_number++;

        }

        if(is_break)
            break;
    }

    create_back_button(engine, main_grid);

    std::cout << "create OK\n";
    return main_grid;
}

void levels_map::create_level_button(uint col, uint row, uint number, const shared_ptr<grid> &main_grid,
                                     const game_engine *engine) {


    shared_ptr <i_controller> btn = make_shared <button>
            (engine->get_window(), button_t::USUAL,
             std::to_string(number));

    engine->get_designer()->wear_controller((btn));
    main_grid->set_controller(btn, {col, row});
}

void levels_map::create_back_button(game_engine *engine,
                              const shared_ptr<grid> &main_grid) {

    shared_ptr <i_controller> back = make_shared <button>
            (engine->get_window(), button_t::USUAL_long, "Back");
    engine->get_designer()->wear_controller((back));
    main_grid->set_controller(back, {0, 6});
}

void levels_map::mouse_pressed_action(game_engine *engine) {
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

void levels_map::process_button_click(game_engine *engine) {
    sf::String button_name =
            std::static_pointer_cast <button> (pressed)->get_label();

    if(button_name == "Back") {
        pressed = nullptr;
        mouse_pressed = false;
        engine->pop_state();
        return;
    }

    std::stringstream stream;
    stream << button_name.toAnsiString();
    uint level_number;
    stream >> level_number;

    engine->load_level(level_number - 1);
    engine->clear_state();
    engine->push_state(play::instance(engine));
}

void levels_map::mouse_released_action() {
    if(pressed) {
        pressed->mouse_released_action();
    }
    pressed = nullptr;
}