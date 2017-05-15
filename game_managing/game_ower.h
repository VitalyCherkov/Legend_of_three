//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_LOSE_H
#define LEGEND_OF_THREE_LOSE_H


#include "i_state.h"

class game_over : public i_state {
public:
    game_over(const game_over &) = delete;
    const game_over &operator=(const game_over &) = delete;
    static game_over* instance(game_engine *engine);

    void init() override {};
    void clear() override {};

    void handle_events(game_engine* engine) override;
    void update(game_engine *engine) override;
    void draw() override;

private:
    game_over(game_engine* engine);
    shared_ptr <grid> create_main_grid(game_engine* engine);
    void mouse_pressed_action(game_engine *engine);
    void process_button_click(game_engine *engine);
    void mouse_released_action();

    bool mouse_pressed;
    shared_ptr <i_controller> main_grid;
    shared_ptr <i_controller> pressed;
};


#endif //LEGEND_OF_THREE_LOSE_H
