//
// Created by vcherkov on 01.05.17.
//

#ifndef LEGEND_OF_THREE_CREATE_ACTION_H
#define LEGEND_OF_THREE_CREATE_ACTION_H

#include "i_action_content.h"
#include "../field/cell_content.h"
#include "../field/game_cell.h"
#include "../utiles.h"
#include <memory>

using std::shared_ptr;

class create_action : virtual public i_action_content {
public:
    create_action(shared_ptr <game_cell> &to,
                  shared_ptr <cell_content> &new_content,
                  float dist = start_dist);

    bool is_finished() const override;
    void update(float time) override;

private:
    static const float start_dist;

    bool started;
    shared_ptr <game_cell> to;
};

#endif //LEGEND_OF_THREE_CREATE_ACTION_H
