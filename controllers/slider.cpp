//
// Created by vcherkov on 03.05.17.
//

#include <iostream>
#include "slider.h"

slider::slider(const shared_ptr<sf::RenderWindow> &window,
               const pair_ii &diapason, int cur_value) :
        i_controller(window),
        mouse_released(true),
        mouse_on(false),
        point_pressed(false),
        diapason(diapason),
        cur_value(cur_value > diapason.first ? cur_value : diapason.first),
        line_sprite(),
        point_sprite(),
        line_texture {nullptr, nullptr},
        point_texture {nullptr, nullptr}
{ }

void slider::set_cur_value(int new_cur_value) {
    cur_value = new_cur_value;
}

void slider::set_diapason(const pair_ii &new_diapason) {
    diapason = new_diapason;

    if(diapason.first > cur_value)
        cur_value = diapason.first;

    if(diapason.second < cur_value)
        cur_value = diapason.second;
}

void slider::set_texture(const pair_texture &line_texture,
                         const pair_texture &point_texture) {
    if(!line_texture.first || !point_texture.first)
        throw std::invalid_argument("slider::set_texture() - first positions of textures cannot be empty");

    this->line_texture = line_texture;
    this->point_texture = point_texture;

    apply_texture(false);
    area::size = sf::Vector2f(line_sprite.getGlobalBounds().width,
                              line_sprite.getGlobalBounds().height);
}

void slider::set_position(const sf::Vector2f &new_position) {
    area::position = new_position;
    line_sprite.setPosition(new_position);
    point_sprite.setPosition(new_position);
}

int slider::get_cur_value() const {
    return cur_value;
}

const slider::pair_ii& slider::get_diapason() const {
    return diapason;
}

void slider::focus_on_action() {
    if(mouse_on)
        return;

    mouse_on = true;
    apply_texture(true);
}

void slider::focus_off_action() {
    if(!mouse_on)
        return;

    mouse_on = false;
    apply_texture(false);
}

void slider::mouse_pressed_action() {
    if(!point_pressed){
        if(!is_point_on_focus()){
            return;
        }

        point_pressed = true;
    }

    float mouse_pos = sf::Mouse::getPosition().x -
            window->getPosition().x;

    cur_value = get_diapason_value(mouse_pos);
    set_point_on_position(mouse_pos);
}

void slider::mouse_released_action() {
    point_pressed = false;
    mouse_released = true;
}

void slider::apply_texture(bool focus_on) {
    if(!focus_on) {
        if(!line_texture.first || !point_texture.first)
            throw std::logic_error("slider:: can not apply empty texture");

        line_sprite.setTexture(*line_texture.first);
        point_sprite.setTexture(*point_texture.first);
    }
    else {
        if(!line_texture.second || !point_texture.second)
            apply_texture(false);

        else {
            line_sprite.setTexture(*line_texture.second);
            point_sprite.setTexture(*point_texture.second);
        }
    }
}

float slider::get_min_bound() const {
    return get_position().x;
}

float slider::get_max_bound() const {
    return get_position().x + get_size().x
           - point_sprite.getGlobalBounds().width;
}

int slider::get_diapason_value(float x_position) const {
    if(x_position <= get_min_bound())
        return diapason.first;

    if(x_position >= get_max_bound())
        return diapason.second;

    if(point_sprite.getGlobalBounds().width == get_size().x)
        return diapason.first;

    int proportion = (diapason.second - diapason.first) /
            (get_size().x - point_sprite.getGlobalBounds().width);

    return proportion * static_cast<int> (x_position);
}

float slider::get_position_of_value(int value) const {
    if(value < diapason.first)
        return get_min_bound();

    if(value > diapason.second)
        return get_max_bound();

    if(diapason.first == diapason.second)
        return get_position().x +
                point_sprite.getGlobalBounds().width / 2.0;

    float proportion = (get_size().x - point_sprite.getGlobalBounds().width) /
            static_cast<float> (diapason.second - diapason.first);

    return position.x + proportion * static_cast<float> (value);
}

void slider::set_point_on_position(float x_position) {
    x_position -= point_sprite.getGlobalBounds().width;

    if(x_position < get_min_bound())
        x_position = get_min_bound();

    if(x_position > get_max_bound())
        x_position = get_max_bound();



    point_sprite.setPosition(x_position, point_sprite.getPosition().y);
}

bool slider::is_mouse_pressed() const {
    return !mouse_released;
}

controller_t slider::get_type() const {
    return controller_t::SLIDER;
}

unsigned int slider::get_rows() const {
    return 1;
}

unsigned int slider::get_cols() const {
    return  4;
}

bool slider::is_point_on_focus() const {
    return point_sprite.getGlobalBounds().contains(
            sf::Mouse::getPosition(*window).x,
            sf::Mouse::getPosition(*window).y
            );

}

void slider::draw() {
    window->draw(line_sprite),
    window->draw(point_sprite);
}



