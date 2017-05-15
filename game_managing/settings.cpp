//
// Created by vcherkov on 08.05.17.
//

#include "settings.h"
#include "exit_program.h"

settings* settings::instance(game_engine *engine) {
    static settings _settings(engine);
    return &_settings;
}

void settings::handle_events(game_engine *engine) {
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

void settings::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
}

void settings::draw() {
    main_grid->draw();
}

settings::settings(game_engine *engine) :
        mouse_pressed(false),
        pressed(nullptr),
        main_grid(nullptr)
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> settings::create_main_grid(game_engine *engine) {
    if(!engine)
        return nullptr;

    shared_ptr <grid> main_grid =
            make_shared <grid> (engine->get_window(), 7, 7);

    create_frames(engine, main_grid);
    create_sliders(engine, main_grid);
    create_buttons(engine, main_grid);

    main_grid->set_position(sf::Vector2f(140, 0));
    return main_grid;
}

void settings::create_frames(game_engine *engine,
                             const shared_ptr<grid> &main_grid) const {
    vector <string> frames = {"Music", "Sounds", "Full Screen", "Unusual"};
    grid::point start_frames{0, 0};
    grid::point size_frames{3, 1};
    for(size_t i = 0; i < frames.size(); i++) {
        shared_ptr <i_controller> frm =
                make_shared <frame> (engine->get_window(), size_frames.col,
                                     size_frames.row, frames[i]);
        engine->get_designer()->wear_controller(frm);
        main_grid->set_controller(frm, start_frames);
        start_frames.row += size_frames.row;
    }
}

void settings::create_sliders(game_engine *engine,
                              const shared_ptr<grid> &main_grid) const {
    shared_ptr <i_controller> s_music =
            make_shared <slider> (engine->get_window());
    main_grid->set_controller(s_music, {3, 0});
    engine->get_designer()->wear_controller(s_music);

    shared_ptr <i_controller> unusual =
            make_shared <switcher> (engine->get_window(), false);
    main_grid->set_controller(unusual, {3, 3});
    engine->get_designer()->wear_controller(unusual);
}

void settings::create_buttons(game_engine *engine,
                              const shared_ptr<grid> &main_grid) const {

    shared_ptr <i_controller> back = make_shared <button>
            (engine->get_window(), button_t::USUAL_long, "Back");
    engine->get_designer()->wear_controller((back));
    main_grid->set_controller(back, {0, 5});
}

void settings::mouse_pressed_action(game_engine *engine) {
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

void settings::process_button_click(game_engine *engine) {
    if(pressed->get_type() == controller_t::BUTTON) {
        sf::String button_name =
                std::static_pointer_cast <button> (pressed)->get_label();
        if(button_name == "Back") {
            pressed = nullptr;
            mouse_pressed = false;
            engine->pop_state();
        }
        return;
    }
    if(pressed->get_type() == controller_t::SWITCHER) {
        engine->get_designer()->set_unusual(
                std::static_pointer_cast <switcher>
                        (pressed)->is_on());
    }
}

void settings::mouse_released_action() {
    if(pressed) {
        pressed->mouse_released_action();
    }
    pressed = nullptr;
}

