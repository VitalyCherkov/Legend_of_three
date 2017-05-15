//
// Created by vcherkov on 03.05.17.
//

#ifndef LEGEND_OF_THREE_CONTROLLER_H
#define LEGEND_OF_THREE_CONTROLLER_H

#include "../area.h"
#include "../utiles.h"

class i_controller : public area {
public:
    i_controller (const std::shared_ptr <sf::RenderWindow> &window = nullptr,
         const sf::Vector2f &position = sf::Vector2f(0,0),
                  const sf::Vector2f &size = sf::Vector2f(0, 0));

    virtual void focus_on_action() = 0;
    virtual void focus_off_action() = 0;
    virtual void mouse_pressed_action() = 0;
    virtual void mouse_released_action() = 0;
    virtual bool is_mouse_pressed() const = 0;
    virtual controller_t get_type() const = 0;

    virtual unsigned int get_rows() const = 0;
    virtual unsigned int get_cols() const = 0;
    virtual void set_position(const sf::Vector2f &position) = 0;

    virtual ~i_controller() = default;
};


#endif //LEGEND_OF_THREE_CONTROLLER_H
