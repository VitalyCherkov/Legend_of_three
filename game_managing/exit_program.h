//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_EXIT_H
#define LEGEND_OF_THREE_EXIT_H

#include "i_state.h"

class exit_program : public i_state {
public:
    exit_program(const exit_program &) = delete;
    const exit_program &operator=(const exit_program &) = delete;
    static exit_program* instance(game_engine *engine);

    void init() override {};
    void clear() override {};

    void handle_events(game_engine* engine) override;
    void update(game_engine* engine) override;
    void draw() override;
private:
    exit_program(game_engine* engine);
    shared_ptr <grid> create_main_grid(game_engine* engine);

    void mouse_pressed_action(game_engine* engine);
    void process_button_click(game_engine *engine);
    void mouse_released_action();

    bool mouse_pressed;
    shared_ptr <i_controller> pressed;
    shared_ptr <i_controller> main_grid;
};


#endif //LEGEND_OF_THREE_EXIT_H
