//
// Created by vcherkov on 03.05.17.
//

#ifndef LEGEND_OF_THREE_BUTTON_H
#define LEGEND_OF_THREE_BUTTON_H

#include "i_controller.h"

using std::shared_ptr;
using std::make_shared;

class button : public i_controller {
    typedef std::pair <shared_ptr <sf::Texture>, shared_ptr <sf::Texture> > pair_texture;

public:
    button(const shared_ptr <sf::RenderWindow> &window = nullptr,
           const button_t &type = button_t::USUAL,
           const sf::String &label = "");

    const sf::String &get_label() const;
    void set_label(const sf::String &new_label);
    void set_style(const sf::Font &font, const sf::Color &color,
            const int size);
    void set_text(const sf::Text &new_text);
    void set_texture(const pair_texture &texture);

    const button_t &get_button_t() const;


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
    void apply_texture(bool focused);

    bool mouse_released;
    bool mouse_on;
    sf::Text label;
    sf::Sprite sprite;
    button_t type;
    pair_texture texture;
};


#endif //LEGEND_OF_THREE_BUTTON_H
