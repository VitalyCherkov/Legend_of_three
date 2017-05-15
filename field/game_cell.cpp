//
// Created by vcherkov on 25.04.17.
//

#include "game_cell.h"
#include "cell_content.h"

game_cell::game_cell(const area &new_area,
                     const p_cell_content &content,
                     const cell_t &type) :
        area(new_area),
        _need_to_change_color(false),
        time_for_color(0),
        _need_to_redesign(true),
        item_pos(-1, -1),
        cell_type(type),
        background(),
        //front(),
        content(content)
{ }

game_cell::game_cell(const game_cell &rhs):
        area(rhs),
        item_pos(-1, -1),
        cell_type(rhs.cell_type),
        background(rhs.background),
        //front(rhs.front),
        content(rhs.content)
{ }

const game_cell& game_cell::operator=(const game_cell &rhs) {
    if(&rhs == this)
        return *this;

    area::operator=(rhs);
    content = rhs.content;
    background = rhs.background;
    cell_type = rhs.cell_type;
    //front = rhs.front;

    return *this;
}

bool game_cell::operator==(const game_cell &rhs) {
    if(!content || !rhs.content)
        return false;

    return *content == *rhs.content;
}

const cell_t& game_cell::get_cell_type() const {
    return cell_type;
}

game_cell::p_cell_content& game_cell::get_content() {
    return content;
}

bool game_cell::is_empty() const {
    return content == nullptr;
}

bool game_cell::is_existing() const {
    return cell_type != cell_t::NONE;
}

bool game_cell::is_free() const {
    return cell_type == cell_t ::FREE ||
            cell_type == cell_t ::INPUT;
}

bool game_cell::is_settable() const {
    return cell_type == cell_t::CAGE ||
            cell_type == cell_t ::INPUT || is_swappable();
}

bool game_cell::is_swappable() const {
    return cell_type == cell_t::ICE1 || cell_type == cell_t::ICE2
       || cell_type == cell_t::FREE;
}

void game_cell::downgrade() {
    if(cell_type != cell_t::BOX1 &&
            cell_type != cell_t::BOX2 &&
            cell_type != cell_t::BOX3 &&
            cell_type != cell_t::BOX4)
        return;

    switch (cell_type) {
        case cell_t::BOX1 :
            cell_type = cell_t::FREE;
            break;

        case cell_t::BOX2 :
            cell_type = cell_t::BOX1;
            break;

        case cell_t::BOX3 :
            cell_type = cell_t::BOX2;
            break;

        case cell_t::BOX4 :
            cell_type = cell_t::BOX3;
            break;

        default:
            break;
    }
    _need_to_redesign = true;
}

void game_cell::clearice() {
    if(cell_type != cell_t::ICE1)
        return;

    cell_type = cell_t::FREE;
    _need_to_redesign = true;
}

void game_cell::open_cage() {
    if(cell_type != cell_t::CAGE)
        return;
    cell_type = cell_t::FREE;
    _need_to_redesign = true;
}

void game_cell::paint_texture() {
    _need_to_change_color = true;
}

void game_cell::set_area(const area &new_area) {
    area::operator=(new_area);
    background.setPosition(position);
    //front.setPosition(position);
}

void game_cell::set_background(const sf::Texture &texture, const sf::IntRect &rect) {
    if(!_need_to_redesign)
        return;
    background.setTexture(texture);
    background.setTextureRect(rect);
    area::size = sf::Vector2f(rect.width, rect.height);
    _need_to_redesign = false;
}

void game_cell::set_front(const sf::Texture &texture) {
    //front.setTexture(texture);
}

void game_cell::set_size(const sf::Vector2f &size) {
    area::size = size;
}

void game_cell::set_content(p_cell_content &content,
                            std::shared_ptr <game_cell> &shared_this) {
    this->content = content;
    this->content->set_parent(shared_this);
}

void game_cell::set_position(const sf::Vector2f &position) {
    area::position = position;
    background.setPosition(position);
    //front.setPosition(position);
}

void game_cell::set_type(const cell_t &type) {
    cell_type = type;
}

void game_cell::create_naked_content(const p_game_cell &shared_this,
                                     const content_t &type,
                                     const bonus_t &bonus) {
    content = std::make_shared <cell_content> (type, bonus, shared_this);
}

void game_cell::clear_content() {
    content = shared_ptr <cell_content> (nullptr);
}

bool game_cell::need_to_redesign() const {
    return _need_to_redesign;
}

void game_cell::draw() {
    if(!window)
        return;
    window->draw(background);
    if(content){
        content->draw();
    }
}

void game_cell::update(float time) {
    if(content)
        content->update(time);
}

void game_cell::update_color(float time) {
    if(!_need_to_change_color)
        return;

    time_for_color += time;
    background.setColor
            (local_colors::red[time_for_color]);

    if((size_t) time_for_color == local_colors::red.size()) {
        background.setColor(sf::Color::White);
        _need_to_change_color = false;
        time_for_color = 0;
    }
}
