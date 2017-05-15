//
// Created by vcherkov on 25.04.17.
//

#ifndef LEGEND_OF_THREE_GAME_FIELD_ITEM_H
#define LEGEND_OF_THREE_GAME_FIELD_ITEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "../area.h"
#include "../utiles.h"
//#include "cell_content.h"

class cell_content;
struct bonus_comb;

class game_cell : public area {
public:
    typedef std::shared_ptr <cell_content> p_cell_content;
    typedef std::shared_ptr <game_cell> p_game_cell;

public:
    game_cell(const area &new_area = area(),
              const p_cell_content &content = p_cell_content (nullptr),
              const cell_t &type = cell_t::NONE);

    game_cell(const game_cell &rhs);
    const game_cell &operator=(const game_cell &rhs);

    bool operator==(const game_cell &rhs);



    const cell_t &get_cell_type() const;
    p_cell_content &get_content();

    bool is_empty() const;
    bool is_existing() const;
    bool is_free() const;
    bool is_settable() const;
    bool is_swappable() const;
    bool need_to_redesign() const;

    void downgrade();
    void clearice();
    void open_cage();

    void paint_texture();
    void set_area(const area &new_area);
    void set_background(const sf::Texture &texture, const sf::IntRect &rect);
    void set_front(const sf::Texture &texture);
    void set_size(const sf::Vector2f &size);
    void set_content(p_cell_content &content,
                     std::shared_ptr <game_cell> &shared_this);
    void set_position(const sf::Vector2f &position);
    void set_type(const cell_t &type);

    void create_naked_content(const p_game_cell &shared_this,
                              const content_t &type,
                              const bonus_t &bonus = bonus_t::NONE);

    void clear_content();

    void draw() override;
    void update(float time);
    void update_color(float time);
private:


    bool _need_to_change_color;
    float time_for_color;
    bool _need_to_redesign;
    sf::Vector2i item_pos;
    cell_t cell_type;
    sf::Sprite background;
    //sf::Sprite front;
    p_cell_content content;

};

#endif //LEGEND_OF_THREE_GAME_FIELD_ITEM_H
