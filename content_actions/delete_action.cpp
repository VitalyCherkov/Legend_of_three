//
// Created by vcherkov on 01.05.17.
//

#include "delete_action.h"

delete_action::delete_action(shared_ptr<game_cell> &from) :
        started(false),
        from(from),
        to(nullptr)
{
    from->paint_texture();
}

bool delete_action::is_finished() const {
    if(!started)
        return false;

    if(!to || to->is_empty())
        return true;

    return to->get_content()->is_stable();
}

void delete_action::update(float time) {
    if(!started){
        started = true;

        if(!from)
            return;

        to = std::make_shared <game_cell> (*from);
        to->set_position(sf::Vector2f(
                to->get_position().x,
                to->get_position().y
                //static_cast<float> (to->get_window()->getSize().y))
        ));

        to->get_content()->set_parent(to);
        from->clear_content();
    }

    to->update(time);
}
