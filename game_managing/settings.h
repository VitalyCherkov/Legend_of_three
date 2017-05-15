//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_SETTINGS_H
#define LEGEND_OF_THREE_SETTINGS_H

#include "i_state.h"

class settings : public i_state {
public:
    settings(const settings &) = delete;
    const settings &operator=(const settings &) = delete;
    static settings* instance(game_engine *engine);

    void init() override {};
    void clear() override {};

    void handle_events(game_engine* engine) override;
    void update(game_engine *engine) override;
    void draw() override;

protected:
    settings(game_engine *engine);
    shared_ptr <grid> create_main_grid(game_engine* engine);
    void create_frames(game_engine* engine,
                       const shared_ptr <grid> &main_grid) const;
    void create_sliders(game_engine* engine,
                       const shared_ptr <grid> &main_grid) const;
    void create_buttons(game_engine* engine,
                        const shared_ptr <grid> &main_grid) const;

    void mouse_pressed_action(game_engine* engine);
    void process_button_click(game_engine* engine);
    void mouse_released_action();

    bool mouse_pressed;
    shared_ptr <i_controller> pressed;
    shared_ptr <i_controller> main_grid;
};

#endif //LEGEND_OF_THREE_SETTINGS_H
