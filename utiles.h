//
// Created by vcherkov on 25.04.17.
//

#ifndef LEGEND_OF_THREE_UTILES_H
#define LEGEND_OF_THREE_UTILES_H

#include <random>
#include <functional>
#include <chrono>
#include <SFML/Graphics/Color.hpp>

enum class direction_t {
    NONE = 0,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct direction {
    static const int dir_x[];
    static const int dir_y[];
    static const uint size;
};

enum class cell_t : char {
    NONE = '\0',
    FREE = '.',
    ICE1 = '"',
    ICE2 = '\'',
    BOX1 = '1',
    BOX2 = '2',
    BOX3 = '3',
    BOX4 = '4',
    CAGE = '#',
    INPUT = '+'
};

enum class content_t {
    NONE = 0,
    STAR,
    BALL,
    TRIANGLE,
    RECT,
    SHAPE
};

enum class bonus_t {
    NONE = 0,
    V,
    H,
    BOMB,
    CROSS,
    MAX_BOMB
};

enum class orientation_t {
    V = 0,
    H
};

enum class controller_t {
    BUTTON = 0,
    SWITCHER,
    SLIDER,
    FRAME,
    GRID
};

enum class button_t {
    TINY = 'T',
    USUAL_long = 'U',
    USUAL = 'u',
    PLAY = 'P',
    RESTART = 'R',
    PAUSE = 'p',
    // SETTINGS = 'S',
    LOCK = 'L',
    UNLOCK = 'l',
    MENU = 'M',
    EXIT = 'E'
};

struct local_colors {
    const static std::vector <sf::Color> red;
};

struct convertation {
    static std::string to_string(const button_t &button);
};

struct random_element{
    static content_t get_content_t();

private:
    static std::function<int()> get_random_content;
};


#endif //LEGEND_OF_THREE_UTILES_H
