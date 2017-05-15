//
// Created by vcherkov on 25.04.17.
//

#ifndef LEGEND_OF_THREE_GAME_FIELD_H
#define LEGEND_OF_THREE_GAME_FIELD_H

#include "game_cell.h"
#include "cell_content.h"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>
#include <queue>
#include <set>
#include <list>
#include "../level.h"
#include "../designer_t.h"

#include "../content_actions/all_content_actions.h"


/*
 *
 *  Здесь и далее некоторые обозначение:
 *
 *  coordinate - игровая координата по столбцам и строкам;
 *  position - координата в пикселях относительно верхнего левого угла;
 *
 *
 * */

class play;

using std::string;
using std::set;
using std::vector;
using std::queue;
using std::shared_ptr;
using std::list;
using sf::Vector2i;
using sf::Vector2f;

class game_field : public area {
    typedef std::pair <int, int> pair_ii;
    typedef set <shared_ptr <i_action_content> > execution_set_t;

public:
    game_field (const area &new_area,
                const level &lvl,
                const shared_ptr <designer_t> &designer);

    void mouse_pressed_reaction();
    void mouse_released_reaction();

    size_t get_rows() const;
    size_t get_cols() const;
    uint get_moves() const;
    uint get_score() const;
    uint get_goal() const;
    bool is_completed() const;

    void update(float time);
    void draw() override;
private:

    void _update(float time);
    void update_execution();

    // ----------------------------------------------------------
    //  Работа с field_condition
    // ----------------------------------------------------------

    void update_field_condition();
    void update_cell_condition(size_t row, size_t col);
    bool is_settable_by_row
            (size_t row, size_t col, const content_t &type) const;
    bool is_settable_by_col
            (size_t row, size_t col, const content_t &type) const;

    // ----------------------------------------------------------
    //  Перемещение по полю
    // ----------------------------------------------------------

    void swap_reaction(const direction_t &dir);

    direction_t get_dir_of_focused_neighbor() const;
    shared_ptr <game_cell>& get_cell_by_direction(const direction_t &dir);

    // ----------------------------------------------------------
    //  Создание поля и нового контента
    // ----------------------------------------------------------

    void construct_field(const level &lvl);

    shared_ptr <cell_content> generate_dressed_content
            (const content_t &type, const bonus_t &bonus = bonus_t::NONE);

    shared_ptr <cell_content> generate_dressed_content_at_first
            (size_t row, size_t col);

    pair_ii get_position_in_focus() const;

    // ----------------------------------------------------------
    //  Работа с бонусом
    // ----------------------------------------------------------

    pair <bonus_t, pair_ii> recognize_bonus
            (size_t row, size_t col) const;
    uint recognize_bonus_by_col(size_t row, size_t col) const;
    bonus_t recognize_bonus_by_res(uint v_number, uint h_number) const;

    void calc_new_bonuses();
    void set_bonus_content(size_t row, size_t col,
                           execution_set_t &execution_set);
    void activate_bonus(set <pair_ii> &bonus_set,
                        const pair_ii &coordinate);

    void activate_bonus_cell(set <pair_ii> &bonus_set,
                             const pair_ii &coordinate);

    void activate_V_bonus(set <pair_ii> &bonus_set,
                          const pair_ii &coordinate);
    void activate_H_bonus(set <pair_ii> &bonus_set,
                          const pair_ii &coordinate);
    void activate_BOMB_bonus(set <pair_ii> &bonus_set,
                          const pair_ii &coordinate);
    void activate_MAX_BOMB_bonus(set <pair_ii> &bonus_set,
                          const pair_ii &coordinate);
    void activate_CROSS_bonus(set <pair_ii> &bonus_set,
                          const pair_ii &coordinate);



    // ----------------------------------------------------------
    //  Сборка сета
    // ----------------------------------------------------------

    void create_execution_set();
    void create_execution_coordinates
            (set <pair_ii> &execution_coordinates);
    void push_same_line_to_execution_set
            (size_t row, size_t col, const orientation_t &o,
             set <pair_ii> &execution_coordinates);

    void create_creation_set();
    void set_upper_content(size_t row, size_t col,
                           execution_set_t &execution_set);

    const pair_ii none_position = {-1, -1};
    pair_ii clicked_position;
    bool mouse_released;
    bool there_is_to_execute;
    int need_to_create;
    map <pair_ii, pair <content_t , bonus_t>> bonuses;
    shared_ptr <designer_t> designer;
    queue <execution_set_t> execution_q;
    vector <vector <shared_ptr <game_cell> > > field;
    vector <vector <pair_ii> > field_condition;

    void set_change_cell(uint row, uint col);
    void set_downgrade_around(uint row, uint col);
    void complete_changes();
    void calculate_change(uint row, uint col);

    set < pair_ii > downdrade_s;
    set < pair_ii > clearice_s;
    set < pair_ii > opencage_s;

    uint goal;
    uint moves;
    uint score;
};

#endif //LEGEND_OF_THREE_GAME_FIELD_H
