//
// Created by vcherkov on 01.05.17.
//

#include "swap_action.h"

swap_action::swap_action(shared_ptr<game_cell> &left,
                           shared_ptr<game_cell> &right) :
        started(false),
        left(left),
        right(right)
{ }

bool swap_action::is_finished() const {
    if(!started)
        return false;

    if(!left || left->is_empty() || !right || right->is_empty())
        return true;

    return left->get_content()->is_stable()
           && right->get_content()->is_stable();
}

void swap_action::update(float time) {
    if(!started) {
        started = true;

        left->get_content()->set_parent(right);
        right->get_content()->set_parent(left);
        std::swap(left->get_content(), right->get_content());
        return;
    }

    left->update(time);
    right->update(time);
}
