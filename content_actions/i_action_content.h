//
// Created by vcherkov on 01.05.17.
//

#ifndef LEGEND_OF_THREE_I_ACTION_CONTENT_H
#define LEGEND_OF_THREE_I_ACTION_CONTENT_H

#include <iostream>

class i_action_content {
public:
    virtual bool is_finished() const = 0;
    virtual void update(float time) = 0;
    virtual ~i_action_content() = default;
};

#endif //LEGEND_OF_THREE_I_ACTION_CONTENT_H
