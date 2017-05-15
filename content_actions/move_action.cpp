//
// Created by vcherkov on 01.05.17.
//

#include "move_action.h"

move_action::move_action(shared_ptr <game_cell> &from, shared_ptr <game_cell> &to) :
        started(false),
        from(from),
        to(to)
{ }

bool move_action::is_finished() const {
    if(!started)
        return false;

    if(!to || to->is_empty())
        return true;

    return to->get_content()->is_stable();
}

void move_action::update(float time) {
    if(!started) {
        started = true;

        from->get_content()->set_parent(to);
        to->get_content() = from->get_content();
    }

    to->update(time);
}