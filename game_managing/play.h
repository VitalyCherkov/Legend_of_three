//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_PLAY_H
#define LEGEND_OF_THREE_PLAY_H

#include "../field/game_field.h"
#include "i_state.h"

class play : public i_state {
public:
    play(const play &) = delete;
    const play &operator=(const play &) = delete;
    static play* instance(game_engine *engine, bool load_level = true);

    void init() override {};
    void clear() override {};

    void load_level(game_engine *engine);
    //void reload_field();
    void update_rest();
    void update_goal();
    void update_score();

    void handle_events(game_engine* engine) override;
    void update(game_engine *engine) override;
    void draw() override;

private:
    play(game_engine* engine);
    shared_ptr <grid> create_main_grid(game_engine* engine);

    void create_level_info(const shared_ptr <grid> &main_grid,
                           const game_engine *engine);
    void create_rest_info(const shared_ptr <grid> &main_grid,
                           const game_engine *engine);
    void create_goal_info(const shared_ptr <grid> &main_grid,
                          const game_engine *engine);
    void create_score_info(const shared_ptr <grid> &main_grid,
                           const game_engine *engine);
    void create_pause_btn(const shared_ptr <grid> &main_grid,
                          const game_engine *engine);



    void mouse_pressed_action(game_engine *engine);
    void process_button_click(game_engine *engine);
    void mouse_released_action();

    static sf::String to_label(const sf::String &name, uint number);

    bool mouse_pressed;
    shared_ptr <i_controller> main_grid;
    shared_ptr <i_controller> pressed;
    shared_ptr <i_controller> level_frm;
    shared_ptr <i_controller> rest_frm;
    shared_ptr <i_controller> goal_frm;
    shared_ptr <i_controller> score_frm;
    shared_ptr <i_controller> pause_btn;
    uint level_number;
    uint start_moves;
    uint rest;
    uint score;

    shared_ptr <game_field> field;
};


#endif //LEGEND_OF_THREE_PLAY_H
