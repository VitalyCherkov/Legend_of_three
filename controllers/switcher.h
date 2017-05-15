//
// Created by vcherkov on 03.05.17.
//

#ifndef LEGEND_OF_THREE_SWITCHER_H
#define LEGEND_OF_THREE_SWITCHER_H

#include "i_controller.h"

using std::shared_ptr;
using std::make_shared;

class switcher : public i_controller {
    typedef std::pair <shared_ptr <sf::Texture>, shared_ptr <sf::Texture> > pair_texture;

public:
    switcher(const shared_ptr <sf::RenderWindow> &window = nullptr,
             bool start_condition = false);

    bool is_on() const;
    void set_texture(const pair_texture &on_texture,
                     const pair_texture &off_texture);

    void focus_on_action() override;
    void focus_off_action() override;
    void mouse_pressed_action() override;
    void mouse_released_action() override;
    bool is_mouse_pressed() const override;
    controller_t get_type() const override;

    unsigned int get_rows() const override;
    unsigned int get_cols() const override;
    void set_position(const sf::Vector2f &position) override;

    void draw() override;
    void update(float time) override {};

private:
    void apply_texture(bool focus_on);
    void apply_on_texture(bool focus_on);
    void apply_off_texture(bool focus_on);

    bool mouse_on;
    bool mouse_released;
    bool condition;
    sf::Sprite sprite;

    pair_texture on_texture;
    pair_texture off_texture;
};


#endif //LEGEND_OF_THREE_SWITCHER_H
