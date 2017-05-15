//
// Created by vcherkov on 03.05.17.
//

#include "button.h"
#include "../global_parameters.h"

button::button(const shared_ptr<sf::RenderWindow> &window,
               const button_t &type, const sf::String &label):
        i_controller(window),
        mouse_released(true),
        mouse_on(false),
        label(),
        sprite(),
        type(type),
        texture {nullptr, nullptr}
{
    this->label.setString(label);
}

const sf::String& button::get_label() const {
    return label.getString();
}

void button::set_label(const sf::String &new_label) {
    label.setString(new_label);
}

void button::set_style(const sf::Font &font, const sf::Color &color, const int size) {
    label.setFont(font);
    label.setFillColor(color);
    label.setCharacterSize(size);

    float width = label.getGlobalBounds().width;
    float height = label.getGlobalBounds().height;
    width = (get_size().x - width) / 2.0;
    height = (get_size().y - height) / 2.0;
    auto pos = get_position();
    pos.x += width;
    pos.y += height;

    label.setPosition(pos);
}

void button::set_text(const sf::Text &new_text) {
    label = new_text;
}

void button::set_texture(const pair_texture &texture) {
    if(!texture.first)
        throw std::invalid_argument("button::set_texture() - first position of texture cannot be empty");

    this->texture = texture;

    apply_texture(false);
    area::size =
            sf::Vector2f(sprite.getGlobalBounds().width,
                         sprite.getGlobalBounds().height);
}

void button::apply_texture(bool focused) {
    if(!focused) {
        if(!texture.first)
            throw std::logic_error("button:: can not apply empty texture");

        sprite.setTexture(*texture.first);
        return;
    }

    if(!texture.second)
        apply_texture(false);
    else
        sprite.setTexture(*texture.second);
}

void button::set_position(const sf::Vector2f &position) {
    area::position = position;
    sprite.setPosition(position);

    float width = label.getGlobalBounds().width;
    float height = label.getGlobalBounds().height;
    width = (get_size().x - width) / 2.0;
    height = (get_size().y - height) / 2.0;
    auto pos = get_position();
    pos.x += width;
    pos.y += height;

    label.setPosition(pos);
}

const button_t& button::get_button_t() const {
    return type;
}

void button::focus_on_action() {
    if(mouse_on)
        return;

    mouse_on = true;
    apply_texture(true);
}

void button::focus_off_action() {
    if(!mouse_on)
        return;

    mouse_on = false;
    apply_texture(false);
}

void button::mouse_pressed_action() {
    if(!mouse_released)
        return;

    mouse_released = false;
    apply_texture(true);
}

void button::mouse_released_action() {
    if(mouse_released)
        return;

    mouse_released = true;
    apply_texture(true);
}

bool button::is_mouse_pressed() const {
    return !mouse_released;
}

controller_t button::get_type() const {
    return controller_t ::BUTTON;
}

unsigned int button::get_rows() const {
    std::string str_type = convertation::to_string(type);

    global_parameters::bounds button_bound =
            global_parameters::get_controller_bounds("button", str_type);

    return button_bound.size.second;
}

unsigned int button::get_cols() const {
    std::string str_type = convertation::to_string(type);

    global_parameters::bounds button_bound =
            global_parameters::get_controller_bounds("button", str_type);

    return button_bound.size.first;
}

void button::draw() {
    if(!window)
        return;

    window->draw(sprite);
    window->draw(label);
}






