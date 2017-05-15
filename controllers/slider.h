//
// Created by vcherkov on 03.05.17.
//

#ifndef LEGEND_OF_THREE_SLIDER_H
#define LEGEND_OF_THREE_SLIDER_H


#include "i_controller.h"

using std::shared_ptr;
using std::make_shared;

class slider : public i_controller {
    typedef std::pair <int, int> pair_ii;
    typedef std::pair <shared_ptr <sf::Texture>,
            shared_ptr <sf::Texture> > pair_texture;


public:
    slider(const shared_ptr <sf::RenderWindow> &window = nullptr,
           const pair_ii &diapason = {0, 100}, int cur_value = 100);

    void set_cur_value(int new_cur_value);
    void set_diapason(const std::pair <int, int> &new_diapason);
    void set_texture(const pair_texture &line_texture,
                     const pair_texture &point_texture);

    int get_cur_value() const;
    const pair_ii &get_diapason() const;

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

    float get_min_bound() const;
    float get_max_bound() const;
    int get_diapason_value(float x_position) const;
    float get_position_of_value(int value) const;

    void set_point_on_position(float x_position);
    bool is_point_on_focus() const;

    bool mouse_released;
    bool mouse_on;
    bool point_pressed;

    pair_ii diapason;
    int cur_value;

    sf::Sprite line_sprite;
    sf::Sprite point_sprite;

    pair_texture line_texture;
    pair_texture point_texture;
};


#endif //LEGEND_OF_THREE_SLIDER_H
