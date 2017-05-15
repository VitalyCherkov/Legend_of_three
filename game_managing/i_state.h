//
// Created by vcherkov on 08.05.17.
//

#ifndef LEGEND_OF_THREE_I_STATE_H
#define LEGEND_OF_THREE_I_STATE_H

#include "game_engine.h"

class i_state{
public:
    virtual void init() = 0;
    virtual void clear() = 0;

    virtual void handle_events(game_engine* engine) = 0;
    virtual void update(game_engine *engine) = 0;
    virtual void draw() = 0;

protected:
    i_state() = default;
    ~i_state() = default;
};

#endif //LEGEND_OF_THREE_I_STATE_H


