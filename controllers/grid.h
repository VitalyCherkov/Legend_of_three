//
// Created by vcherkov on 03.05.17.
//

#ifndef LEGEND_OF_THREE_GRID_H
#define LEGEND_OF_THREE_GRID_H

#include "../area.h"
#include "i_controller.h"
#include <set>
#include <vector>

using std::vector;
using std::set;
using std::shared_ptr;
using std::make_shared;

class grid : public i_controller {
    typedef unsigned int uint;
    typedef std::pair <int, int> pair_ii;
    typedef std::pair <uint, uint> upair_ii;

public:
    struct point {unsigned int col; unsigned int row; };

    grid(const shared_ptr <sf::RenderWindow> &window = nullptr,
         uint cols = 0, uint rows = 0);

    bool is_free_point(const point &pt) const;

    grid::point get_local_coords(const shared_ptr <i_controller> &controller) const;
    sf::Vector2f get_point_position(const point &coordinate) const;
    const std::shared_ptr <i_controller> &get_controller_in_focus();

    bool set_controller(const shared_ptr <i_controller> &controller,
                        const point &coordinate);


    void erase_controller(const shared_ptr <i_controller> &controller);

    void focus_on_action() override;
    void focus_off_action() override;
    void mouse_pressed_action() override;
    void mouse_released_action() override;
    bool is_mouse_pressed() const override {return false;};
    controller_t get_type() const override;

    const shared_ptr <i_controller> &get_controller_in_focus() const;
    unsigned int get_rows() const override;
    unsigned int get_cols() const override;
    void set_position(const sf::Vector2f &position) override;

    void draw() override;
    void update(float time) override;

private:
    bool is_free_rect(const point &coords,
                      const point &size) const;
    void fill_rect(const point &coords,
                   const point &size, bool value = true);

    uint cols;
    uint rows;

    shared_ptr <i_controller> none_controller;
    set <shared_ptr <i_controller> > container;
    vector < vector <bool> > points;
};


#endif //LEGEND_OF_THREE_GRID_H
