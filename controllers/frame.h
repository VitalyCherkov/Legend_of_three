//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_TEXT_H
#define LEGEND_OF_THREE_TEXT_H

#include "../global_parameters.h"
#include "i_controller.h"

using std::shared_ptr;
using std::make_shared;

class frame : public i_controller{
public:

    frame(const shared_ptr <sf::RenderWindow> &window = nullptr,
         uint cols = 0, uint rows = 0, const sf::String &data = "");

    const sf::String &get_label() const;
    void set_data(const sf::String &new_label);
    void set_style(const sf::Font &font, const sf::Color &color,
                   const uint size);
    void set_text(const sf::Text &new_text);
    void set_texture(const shared_ptr <sf::Texture> &texture);

    void focus_on_action() override {};
    void focus_off_action() override {};
    void mouse_pressed_action() override {};
    void mouse_released_action() override {};
    bool is_mouse_pressed() const override { return false; };
    controller_t get_type() const override;

    unsigned int get_rows() const override;
    unsigned int get_cols() const override;
    void set_position(const sf::Vector2f &position) override;

    void draw() override;
    void update(float time) override {};

private:
    void apply_texture();

    uint cols;
    uint rows;

    sf::Text data;
    sf::Sprite sprite;
    shared_ptr <sf::Texture> texture;


};


#endif //LEGEND_OF_THREE_TEXT_H
