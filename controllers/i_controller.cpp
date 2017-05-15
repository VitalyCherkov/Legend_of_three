//
// Created by vcherkov on 03.05.17.
//

#include "i_controller.h"

i_controller::i_controller(const std::shared_ptr<sf::RenderWindow> &window,
                           const sf::Vector2f &position,
                           const sf::Vector2f &size) :
        area(window, position, size)
{}
