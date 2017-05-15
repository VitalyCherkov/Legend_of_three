//
// Created by vcherkov on 26.04.17.
//

#include "utiles.h"

const int direction::dir_x[] = {0, 0, 1, 0, -1};
const int direction::dir_y[] = {0, -1, 0, 1, 0};
const uint direction::size = 5;

const std::vector <sf::Color> local_colors::red {
        sf::Color(229, 57, 53),
        sf::Color(244, 67, 54),
        sf::Color(239, 83, 80),
        sf::Color(229, 115, 115),
        sf::Color(239, 154, 154),
        sf::Color(255, 205, 210),
        sf::Color(255, 235, 238),
        sf::Color::White,
        sf::Color::White
};

std::string convertation::to_string(const button_t &button) {
    switch (button) {
        case button_t::TINY :
            return "tiny";
        case button_t::USUAL_long :
            return "usual_long";
        case button_t::USUAL :
            return "usual";
        case button_t::PLAY :
            return "play";
        case button_t::RESTART :
            return "restart";
        case button_t::PAUSE :
            return "pause";
        case button_t::LOCK :
            return "lock";
        case button_t::UNLOCK :
            return "unlock";
        case button_t::EXIT :
            return "exit";
        case button_t::MENU :
            return "menu";
        default:
            return "usual";
    }
}

content_t random_element::get_content_t() {
    //int rnd = get_random_content();
    return static_cast <content_t> (get_random_content());
}

std::function<int()> random_element::get_random_content = std::bind(
        std::uniform_int_distribution<int> (1, 5),
        std::default_random_engine (
                static_cast<unsigned long> (
                        std::chrono::system_clock::now().time_since_epoch().count()
                )
        )
);
