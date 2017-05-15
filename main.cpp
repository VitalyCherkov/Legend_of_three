#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>

#include "controllers/all_controllers.h"
#include "field/game_field.h"
#include "game_managing/game_engine.h"

using std::shared_ptr;
using std::make_shared;

int main() {

    game_engine game("Legend of three", 1000, 800);

    while (game.running()) {
        game.handle_events();
        game.update();
        game.draw();
    }

    return 0;
}