//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_MENU_H
#define LEGEND_OF_THREE_MENU_H

#include "i_state.h"

class menu : public i_state {
public:
    menu(const menu &) = delete;
    const menu &operator=(const menu &) = delete;
    static menu* instance(game_engine *engine);

    void init() override {};
    void clear() override {};

    void handle_events(game_engine* engine) override;
    void update(game_engine *engine) override;
    void draw() override;
private:
    menu(game_engine* engine);
    shared_ptr <grid> create_main_grid(game_engine* engine);
    void mouse_pressed_action(game_engine *engine);
    void process_button_click(game_engine *engine);
    void mouse_released_action();

    bool mouse_pressed;
    shared_ptr <i_controller> main_grid;
    shared_ptr <i_controller> pressed;
    vector <shared_ptr <i_controller> > buttons;
};


#endif //LEGEND_OF_THREE_MENU_H
