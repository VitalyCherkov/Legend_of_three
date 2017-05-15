//
// Created by vcherkov on 01.05.17.
//

#ifndef LEGEND_OF_THREE_SWAP_CONTENT_H
#define LEGEND_OF_THREE_SWAP_CONTENT_H


#include "i_action_content.h"
#include "../field/cell_content.h"
#include "../field/game_cell.h"
#include "../utiles.h"
#include <memory>

using std::shared_ptr;

class swap_action : virtual public i_action_content {
public:
    swap_action(shared_ptr <game_cell> &left,
                shared_ptr <game_cell> &right);

    bool is_finished() const override;
    void update(float time) override;

private:
    bool started;
    shared_ptr <game_cell> &left;
    shared_ptr <game_cell> &right;
};

#endif //LEGEND_OF_THREE_SWAP_CONTENT_H
