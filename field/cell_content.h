//
// Created by vcherkov on 26.04.17.
//

#ifndef LEGEND_OF_THREE_CELL_CONTENT_H
#define LEGEND_OF_THREE_CELL_CONTENT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "../utiles.h"
#include "game_cell.h"

#include <iostream>

using std::shared_ptr;
using std::weak_ptr;

class cell_content {
public:
    cell_content(const content_t &type = content_t::NONE,
                 const bonus_t &bonus = bonus_t::NONE,
                 const shared_ptr <game_cell> &parent =
                    shared_ptr <game_cell> (nullptr)
    );

    cell_content(const cell_content &rhs);
    const cell_content &operator=(const cell_content &rhs);

    bool is_dressed() const;
    bool is_stable() const;
    const content_t &get_content_type() const;
    const bonus_t &get_bonus() const;

    bool operator==(const cell_content &rhs);

    shared_ptr <game_cell> get_parent();

    void set_position(const sf::Vector2f &position);
    void set_bonus(const bonus_t &bonus);
    void set_texture(const sf::Texture &texture, const sf::IntRect &rect);
    void set_bonus_texture(const sf::Texture &bonus_texture,
                           const sf::IntRect &rect);
    void set_parent(shared_ptr <game_cell> &parent);

    void draw();
    void update(float time);

private:
    static float speed;
    const float eps = 10.0;

    weak_ptr <game_cell> parent;

    bool dressed;
    bool bonus_dressed;

    content_t type;
    bonus_t bonus;

    sf::Sprite sprite;
    sf::Sprite bonus_sprite;
};

#endif //LEGEND_OF_THREE_CELL_CONTENT_H
