//
// Created by vcherkov on 26.04.17.
//

#include "cell_content.h"
#include <math.h>

float cell_content::speed = 10;

cell_content::cell_content(const content_t &type, const bonus_t &bonus,
                           const shared_ptr <game_cell> &parent) :
        parent(parent),
        dressed(false),
        bonus_dressed(false),
        type(type),
        bonus(bonus),
        sprite(),
        bonus_sprite()
{
    if(this->parent.lock()) {
        sprite.setPosition(this->parent.lock()->get_position());
        bonus_sprite.setPosition(this->parent.lock()->get_position());
    }
}

cell_content::cell_content(const cell_content &rhs) :
    type(rhs.type),
    bonus(rhs.bonus),
    bonus_dressed(rhs.bonus_dressed),
    dressed(rhs.dressed),
    sprite(rhs.sprite),
    bonus_sprite(rhs.bonus_sprite),
    parent(rhs.parent)
{ }

const cell_content& cell_content::operator=(const cell_content &rhs) {
    if(&rhs == this)
        return *this;

    type = rhs.type;
    bonus = rhs.bonus;
    dressed = rhs.dressed;
    bonus_dressed = rhs.bonus_dressed;
    sprite = rhs.sprite;
    bonus_sprite = rhs.bonus_sprite;
    parent = rhs.parent;

    return *this;
}

const bonus_t& cell_content::get_bonus() const {
    return bonus;
}

const content_t& cell_content::get_content_type() const {
    return type;
}

bool cell_content::is_dressed() const {
    return dressed;
}

bool cell_content::is_stable() const {
    return sprite.getPosition() == parent.lock()->get_position();
}

bool cell_content::operator==(const cell_content &rhs) {
    return type == rhs.type;
}

std::shared_ptr <game_cell> cell_content::get_parent() {
    return parent.lock();
}

void cell_content::set_position(const sf::Vector2f &position) {
    sprite.setPosition(position);
    bonus_sprite.setPosition(position);
}

void cell_content::set_bonus(const bonus_t &bonus) {
    this->bonus = bonus;
}

void cell_content::set_texture(const sf::Texture &texture,
                               const sf::IntRect &rect) {
    dressed = true;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
}

void cell_content::set_bonus_texture(const sf::Texture &texture,
                               const sf::IntRect &rect) {
    bonus_dressed = true;
    bonus_sprite.setTexture(texture);
    bonus_sprite.setTextureRect(rect);
}

void cell_content::set_parent(std::shared_ptr<game_cell> &parent) {
    this->parent = parent;
}

void cell_content::draw() {
    if(!parent.lock())
        return;

    if(bonus != bonus_t::NONE)
        parent.lock()->get_window()->draw(bonus_sprite);

    parent.lock()->get_window()->draw(sprite);
}

void cell_content::update(float time) {
    if(!parent.lock())
        return;

    if (parent.lock()->get_position() == sprite.getPosition())
        return;

    sf::Vector2f way(
            parent.lock()->get_position().x - sprite.getPosition().x,
            parent.lock()->get_position().y - sprite.getPosition().y
    );

    float dist = sqrtf(way.x * way.x + way.y * way.y);
    float move_dist = speed * time;

    if(move_dist >= dist || dist <= eps) {
        sprite.setPosition(parent.lock()->get_position());
        bonus_sprite.setPosition(parent.lock()->get_position());
        return;
    }

    sprite.move(move_dist * way.x / dist, move_dist * way.y / dist);
    bonus_sprite.move(move_dist * way.x / dist, move_dist * way.y / dist);
}

