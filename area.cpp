//
// Created by vcherkov on 22.04.17.
//

#include <iostream>
#include "area.h"


area::area(const std::shared_ptr <sf::RenderWindow> &window,
           const sf::Vector2f &position, const sf::Vector2f &size):
    window(window),
    position(position),
    size(size)
{}

area::area(const area &rhs):
    window(rhs.window),
    position(rhs.position),
    size(rhs.size)
{}

const area& area::operator=(const area &rhs) {
    if(this == &rhs)
        return *this;

    position = rhs.position;
    size = rhs.size;
    window = rhs.window;

    return *this;
}

const sf::Vector2f& area::get_position() const {
    return position;
}
const sf::Vector2f& area::get_size() const {
    return size;
}
float area::get_height() const {
    return size.y;
}
float area::get_width() const {
    return size.x;
}
std::shared_ptr <sf::RenderWindow>& area::get_window() {
    return window;
}

bool area::is_focus_on() const {

    if(sf::FloatRect(position.x, position.y, size.x, size.y).contains(
            sf::Mouse::getPosition(*window).x,
            sf::Mouse::getPosition(*window).y)
            ){
        return true;
    }

    return false;
}

void area::set_window(const std::shared_ptr<sf::RenderWindow> &window) {
    this->window = window;
}



