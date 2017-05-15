//
// Created by vcherkov on 03.05.17.
//

#include <iostream>
#include "switcher.h"

switcher::switcher(const shared_ptr<sf::RenderWindow> &window,
                   bool start_condition) :
        i_controller(window),
        mouse_on(false),
        mouse_released(true),
        condition(start_condition)
{ }

bool switcher::is_on() const {
    return condition;
}

void switcher::set_texture(const pair_texture &on_texture,
                           const pair_texture &off_texture) {
    if(!off_texture.first || !on_texture.first)
        throw std::invalid_argument("switcher::set_texture() - first positions of textures cannot be empty");

    this->on_texture = on_texture;
    this->off_texture = off_texture;

    apply_texture(false);

    area::size = sf::Vector2f(sprite.getGlobalBounds().width,
                              sprite.getGlobalBounds().height);

}

void switcher::set_position(const sf::Vector2f &position) {
    area::position = position;
    sprite.setPosition(position);
}

void switcher::focus_on_action() {
    if(mouse_on)
        return;

    mouse_on = true;
    apply_texture(true);
}

void switcher::focus_off_action() {
    if(!mouse_on)
        return;

    mouse_on = false;
    apply_texture(false);
}

void switcher::mouse_pressed_action() {
    if(!mouse_released)
        return;

    mouse_released = false;
    condition = !condition;

    apply_texture(true);
}

void switcher::mouse_released_action() {
    if(mouse_released)
        return;

    apply_texture(true);
    mouse_released = true;
}

bool switcher::is_mouse_pressed() const {
    return !mouse_released;
}

controller_t switcher::get_type() const {
    return controller_t::SWITCHER;
}

unsigned int switcher::get_rows() const {
    return 1;
}

unsigned int switcher::get_cols() const {
    return 2;
}

void switcher::apply_texture(bool focus_on) {
    if(condition)
        apply_on_texture(focus_on);
    else
        apply_off_texture(focus_on);
}

void switcher::apply_on_texture(bool focus_on) {
    if(!focus_on) {
        if(!on_texture.first)
            throw std::logic_error("switcher:: can not apply empty texture");

        sprite.setTexture(*on_texture.first);
    }
    else {
        if(!on_texture.second)
            apply_on_texture(false);

        else sprite.setTexture(*on_texture.second);

    }
}

void switcher::apply_off_texture(bool focus_on) {
    if(!focus_on) {
        if(!off_texture.first)
            throw std::logic_error("switcher:: can not apply empty texture");

        sprite.setTexture(*off_texture.first);
    }
    else {
        if(!off_texture.second)
            apply_on_texture(false);

        else sprite.setTexture(*off_texture.second);
    }
}

void switcher::draw() {
    if(!window)
        return;

    window->draw(sprite);
}