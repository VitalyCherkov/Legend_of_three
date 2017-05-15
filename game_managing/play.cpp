//
// Created by vcherkov on 08.05.17.
//

#include "win.h"
#include "game_ower.h"
#include "settings.h"
#include "exit_program.h"
#include "play.h"
#include "pause.h"

play* play::instance(game_engine *engine, bool load_level) {
    static play _play(engine);
    if(load_level)
        _play.load_level(engine);
    return &_play;
}

void play::update_score() {
    if(field->get_score() != score) {
        score = field->get_score();
        std::static_pointer_cast <frame> (score_frm)->set_data(
                to_label("Score: ", score)
        );
    }
}

void play::update_rest() {
    if(rest + field->get_moves() != start_moves) {
        rest--;
        std::static_pointer_cast <frame> (rest_frm)->set_data(
                to_label("Rest of moves: ", rest)
        );
    }
}

void play::update_goal() {
    if(field->get_goal()) {
        std::static_pointer_cast <frame> (goal_frm)->set_data(
                to_label("Goal: ", field->get_goal())
        );
    }
}

void play::handle_events(game_engine *engine) {
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

void play::update(game_engine *engine) {
    main_grid->focus_on_action();
    main_grid->focus_off_action();
    main_grid->update(engine->get_time());
    field->update(engine->get_time() / 40.0);
    update_rest();
    update_goal();
    update_score();

    if(field->is_completed()) {
        if(!field->get_goal()) {
            engine->clear_state();
            engine->push_state(win::instance(engine));
            return;
        }

        if(!rest) {
            engine->clear_state();
            engine->push_state(game_over::instance(engine));
            return;
        }
    }
}

void play::draw() {
    main_grid->draw();
    if(field)
        field->draw();
}

play::play(game_engine *engine) :
    mouse_pressed(false),
    main_grid(nullptr),
    pressed(nullptr),
    level_frm(nullptr),
    rest_frm(nullptr),
    score_frm(nullptr),
    pause_btn(nullptr),
    level_number(0),
    start_moves(0),
    rest(0),
    score(0),
    field(nullptr)
{
    main_grid = create_main_grid(engine);
}

shared_ptr <grid> play::create_main_grid(game_engine *engine) {
    if(!engine)
        return nullptr;

    shared_ptr <grid> main_grid =
            make_shared <grid> (engine->get_window(), 6, 13);

    create_level_info(main_grid, engine);
    create_rest_info(main_grid, engine);
    create_goal_info(main_grid, engine);
    create_score_info(main_grid, engine);
    create_pause_btn(main_grid, engine);

    main_grid->set_position(sf::Vector2f(0, 0));

    return main_grid;
}

void play::create_level_info(const shared_ptr<grid> &main_grid,
                             const game_engine *engine) {
    level_frm = make_shared <frame> (engine->get_window(), 6, 1,
                                     to_label("Level: ",
                                              level_number));

    engine->get_designer()->wear_controller(level_frm);
    main_grid->set_controller(level_frm, {0, 0});
}

void play::create_rest_info(const shared_ptr<grid> &main_grid,
                            const game_engine *engine) {
    rest_frm = make_shared <frame> (engine->get_window(), 6, 2,
                                    to_label("Rest of moves: ",
                                             rest));
    engine->get_designer()->wear_controller(rest_frm);
    main_grid->set_controller(rest_frm, {0, 1});
}

void play::create_goal_info(const shared_ptr<grid> &main_grid,
                            const game_engine *engine) {
    goal_frm = make_shared <frame> (engine->get_window(), 6, 2,
                                    to_label("Goal: ",
                                             0));
    engine->get_designer()->wear_controller(goal_frm);
    main_grid->set_controller(goal_frm, {0, 4});
}

void play::create_score_info(const shared_ptr<grid> &main_grid,
                             const game_engine *engine) {
    score_frm = make_shared <frame> (engine->get_window(), 6, 1,
                                     to_label("Score: ", score));
    engine->get_designer()->wear_controller(score_frm);
    main_grid->set_controller(score_frm, {0, 8});
}

void play::create_pause_btn(const shared_ptr<grid> &main_grid, const game_engine *engine) {
    pause_btn = make_shared <button> (engine->get_window(),
                                      button_t::PAUSE);
    engine->get_designer()->wear_controller(pause_btn);
    main_grid->set_controller(pause_btn, {2, 11});
}

void play::load_level(game_engine *engine) {
    field = make_shared <game_field> (
            area(engine->get_window(),
                 sf::Vector2f(210, 0),
                 sf::Vector2f(600, 600)),
            engine->get_level(),
            engine->get_designer()
    );

    level_number = engine->get_level().get_level_number();
    rest = start_moves = engine->get_level().get_moves();
    score = 0;

    std::static_pointer_cast <frame> (level_frm)->set_data(
            to_label("Level: ", level_number));
    std::static_pointer_cast <frame> (rest_frm)->set_data(
            to_label("Rest of moves: ", rest));
    std::static_pointer_cast <frame> (score_frm)->set_data(
            to_label("Score: ", score));
}

void play::mouse_pressed_action(game_engine *engine) {
    if(field && field->is_focus_on()) {
        field->mouse_pressed_reaction();
        return;
    }

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

void play::process_button_click(game_engine *engine) {
    if(pressed->get_type() == controller_t::BUTTON) {
        pressed = nullptr;
        mouse_pressed = false;
        engine->push_state(pause::instance(engine));
    }
}

void play::mouse_released_action() {
    main_grid->mouse_released_action();
    field->mouse_released_reaction();
    pressed = nullptr;
}

sf::String play::to_label(const sf::String &name, uint number) {
    sf::String label(std::to_string(number));
    return name + label;
}
