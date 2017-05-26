//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_LEVELS_H
#define LEGEND_OF_THREE_LEVELS_H


#include "i_state.h"

class levels_map : public i_state {
public:
    levels_map(const levels_map &) = delete;
    const levels_map &operator=(const levels_map &) = delete;
    static levels_map* instance(game_engine *engine);

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
    levels_map(game_engine* engine);
    shared_ptr <grid> create_main_grid(game_engine* engine);

    void create_level_button(uint col, uint row, uint number, const shared_ptr <grid> &main_grid,
                           const game_engine *engine);
    void create_back_button(game_engine *engine,
                            const shared_ptr<grid> &main_grid);

    void mouse_pressed_action(game_engine *engine);
    void process_button_click(game_engine *engine);
    void mouse_released_action();

    bool mouse_pressed;
    shared_ptr <i_controller> main_grid;
    shared_ptr <i_controller> pressed;
};


#endif //LEGEND_OF_THREE_LEVELS_H
