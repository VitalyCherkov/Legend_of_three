//
// Created by vcherkov on 01.05.17.
//

#include "create_action.h"

const float create_action::start_dist = 100.0;

create_action::create_action(shared_ptr <game_cell> &to,
                             shared_ptr <cell_content> &new_content, float dist) :
        started(false),
        to(to)
{
    if(!to || !new_content)
        return;

    this->to->get_content() = new_content;
    this->to->get_content()->set_parent(this->to);

    new_content->set_position(sf::Vector2f(to->get_position().x,
                                           to->get_position().y - dist));
}

bool create_action::is_finished() const {
    if(!started)
        return false;

    if(!to || to->is_empty())
        return true;

    return to->get_content()->is_stable();
}

void create_action::update(float time) {
    if(!started) {
        started = true;
    }

    to->update(time);
}