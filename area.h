//
// Created by vcherkov on 22.04.17.
//

#ifndef LEGEND_OF_THREE_AREA_H
#define LEGEND_OF_THREE_AREA_H

#include <memory>
#include <SFML/Graphics.hpp>

class area {
public:
    area(const std::shared_ptr <sf::RenderWindow> &window = nullptr,
         const sf::Vector2f &position = sf::Vector2f(0, 0),
         const sf::Vector2f &size = sf::Vector2f(0, 0));


    area(const area &rhs);
    const area &operator=(const area &rhs);

    const sf::Vector2f &get_position() const;
    const sf::Vector2f &get_size() const;
    float get_height() const;
    float get_width() const;
    std::shared_ptr <sf::RenderWindow> &get_window();

    bool is_focus_on() const;

    void set_window(const std::shared_ptr <sf::RenderWindow> &window);

    virtual void draw(){};
    virtual void update(float time){};
    virtual ~area() = default;
protected:
    std::shared_ptr <sf::RenderWindow> window;

    sf::Vector2f position;
    sf::Vector2f size;
};

#endif //LEGEND_OF_THREE_AREA_H
