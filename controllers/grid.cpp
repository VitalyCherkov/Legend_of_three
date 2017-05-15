//
// Created by vcherkov on 03.05.17.
//

#include "grid.h"
#include "../global_parameters.h"
#include <iostream>

grid::grid(const shared_ptr<sf::RenderWindow> &window, uint cols, uint rows) :
    i_controller(window),
    cols(cols),
    rows(rows),
    none_controller(nullptr),
    container(),
    points(rows)
{
    area::position = sf::Vector2f(0, 0);

    sf::Vector2f point_size = global_parameters::get_point_size();

    area::size = sf::Vector2f(
            point_size.x * static_cast <float> (cols),
            point_size.y * static_cast <float> (rows)
    );

    for(uint i = 0; i < rows; i++) {
        points[i].resize(cols);
    }
}

bool grid::is_free_point(const point &pt) const {
    return !points[pt.row][pt.col];
}

grid::point grid::get_local_coords
        (const shared_ptr<i_controller> &controller) const {
    if(!controller)
        throw std::invalid_argument("grid::get_local_coords() - empty argument");

    if(container.find(controller) == container.end())
        throw std::logic_error("grid::get_local_coords() - wrong controller");

    float col = controller->get_position().x - get_position().x;
    col /= global_parameters::get_point_size().x;

    float row = controller->get_position().y - get_position().y;
    row /= global_parameters::get_point_size().y;

    return {
            static_cast <uint> (col),
            static_cast <uint> (row)
    };
}

sf::Vector2f grid::get_point_position(const point &coordinate) const {
    if(coordinate.col > get_cols() || coordinate.row > get_rows())
        throw std::invalid_argument("grid::get_global_coords() - the point is not in the cage");

    sf::Vector2f pos = area::position;
    pos.x += static_cast<float> (coordinate.col)
             * global_parameters::get_point_size().x;
    pos.y += static_cast<float> (coordinate.row)
             * global_parameters::get_point_size().y;

    return pos;
}

const std::shared_ptr <i_controller>& grid::get_controller_in_focus() {
    for(auto &it : container)
        if(it->is_focus_on())
            return it;

    return none_controller;
}

bool grid::set_controller(const shared_ptr<i_controller> &controller,
                          const point &coordinate) {
    if(!controller)
        return false;


    if(!is_free_rect(coordinate,
                     {controller->get_cols(), controller->get_rows()}))
        return false;

    container.insert(controller);
    fill_rect(coordinate, {controller->get_cols(), controller->get_rows()});
    sf::Vector2f pos = get_point_position(coordinate);
    controller->set_position(pos);

    return true;
}

void grid::erase_controller(const shared_ptr<i_controller> &controller) {
    if(container.find(controller) == container.end())
        return;

    point local_coords = get_local_coords(controller);

    fill_rect(local_coords,
              {controller->get_cols(), controller->get_rows()}, false);

    container.erase(controller);
}

void grid::focus_on_action() {
    for(auto &it : container) {
        if(it->is_focus_on())
            it->focus_on_action();
    }
}

void grid::focus_off_action() {
    for(auto &it : container) {
        if(!it->is_focus_on())
            it->focus_off_action();
    }
}

void grid::mouse_pressed_action() {
    for(auto &it : container) {
        if(it->is_focus_on())
            mouse_pressed_action();
    }
}

void grid::mouse_released_action(){
    for(auto &it : container) {
        if(it->is_focus_on())
            mouse_released_action();
    }
}

controller_t grid::get_type() const {
    return controller_t::GRID;
}

const shared_ptr <i_controller> &
grid::get_controller_in_focus() const {
    for(auto &it : container) {
        if(it->is_focus_on())
            return it;
    }
    return none_controller;
}

unsigned int grid::get_rows() const {
    return rows;
}

unsigned int grid::get_cols() const {
    return cols;
}

void grid::set_position(const sf::Vector2f &position) {
    float delta_x = position.x - this->position.x;
    float delta_y = position.y - this->position.y;

    for(auto &it : container)
        it->set_position(
                sf::Vector2f(
                        it->get_position().x + delta_x,
                        it->get_position().y + delta_y
                )
        );

    this->position = position;
}

void grid::draw() {
    for(auto &it : container)
        it->draw();
}

void grid::update(float time) {
    for(auto &it : container)
        it->update(time);
}

bool grid::is_free_rect(const point &coords,
                        const point &size) const {

    if(coords.col + size.col > get_cols()
       || coords.row + size.row > get_rows())
        return false;



    for(uint i = coords.row; i < coords.row + size.row; i++) {
        for (uint j = coords.col; j < coords.col + size.col; j++) {
            if(!is_free_point({j, i}))
                return false;
        }
    }
    return true;
}

void grid::fill_rect(const point &coords,
                     const point &size, bool value) {
    if(coords.col + size.col > get_cols()
       || coords.row + size.row > get_rows())
        return;

    for(size_t i = coords.row; i < coords.row + size.row; i++) {
        for (size_t j = coords.col; j < coords.col + size.col; j++) {
            points[i][j] = value;
        }
    }
}