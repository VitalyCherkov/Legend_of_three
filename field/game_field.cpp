//
// Created by vcherkov on 25.04.17.
//

#include "game_field.h"
#include "../game_managing/play.h"

game_field::game_field(const area &new_area, const level &lvl,
                       const shared_ptr <designer_t> &designer) :
        area(new_area),
        clicked_position(none_position),
        mouse_released(true),
        there_is_to_execute(false),
        need_to_create(lvl.get_rows()),
        designer(designer),
        execution_q(),
        field(lvl.get_rows()),
        field_condition(lvl.get_rows()),
        create_distance(lvl.get_cols()),
        downdrade_s(),
        clearice_s(),
        opencage_s(),
        goal(0),
        moves(0),
        score(0)
{
    for(size_t i = 0; i < lvl.get_rows(); i++) {
        field[i].resize(lvl.get_cols());
        field_condition[i].resize(lvl.get_cols());
    }
    need_to_create *= get_cols();

    construct_field(lvl);
}

void game_field::mouse_pressed_reaction() {
    if(mouse_released) {
        mouse_released = false;

        if(!execution_q.empty())
            return;

        clicked_position = get_position_in_focus();
    }

    if(clicked_position != none_position)
        swap_reaction(get_dir_of_focused_neighbor());
}

void game_field::mouse_released_reaction() {
    mouse_released = true;
    clicked_position = none_position;
}

size_t game_field::get_rows() const {
    return field.size();
}

size_t game_field::get_cols() const {
    return field.size() ? field[0].size() : 0;
}

uint game_field::get_moves() const {
    return moves;
}

uint game_field::get_score() const {
    return score;
}

uint game_field::get_goal() const {
    return goal;
}

bool game_field::is_completed() const {
    return execution_q.empty();
}

void game_field::update(float time) {
    if(execution_q.empty())
        return;

    _update(time);
    update_execution();
}

void game_field::draw() {
    for(size_t i = 0; i < get_rows(); i++)
        for(size_t j = 0; j < get_cols(); j++)
            field[i][j]->draw();
}

void game_field::_update(float time) {
    for(uint i = 0; i < get_rows(); i++)
        for(uint j = 0; j < get_cols(); j++)
            field[i][j]->update_color(time);

    for(auto it = execution_q.front().begin();
        it != execution_q.front().end();) {

        (*it)->update(time);
        if((*it)->is_finished()) {
            it = execution_q.front().erase(it);
            if(there_is_to_execute)
                score += 10;
        }
        else
            it++;
    }
}

void game_field::update_execution() {
    if(execution_q.front().empty())
        execution_q.pop();
    else
        return;

    if(there_is_to_execute) {
        there_is_to_execute = false;
        create_execution_set();
    }
    else {
        create_creation_set();
        update_field_condition();
    }

}

void game_field::update_field_condition() {
    for(size_t i = 0; i < get_rows(); i++)
        for (size_t j = 0; j < get_cols(); j++)
            update_cell_condition(i, j);
    complete_changes();
    calc_new_bonuses();
    create_distance.clear();
    create_distance.resize(get_cols());
}


void game_field::update_cell_condition(size_t row, size_t col) {
    if(!field[row][col] || field[row][col]->is_empty())
        return;

    if(row && field[row - 1][col] && *field[row][col] == *field[row - 1][col])
        field_condition[row][col].first =
                field_condition[row - 1][col].first + 1;
    else field_condition[row][col].first = 1;

    if(col && field[row][col - 1] && *field[row][col] == *field[row][col - 1])
        field_condition[row][col].second =
                field_condition[row][col - 1].second + 1;
    else field_condition[row][col].second = 1;

    if(field_condition[row][col].first > 2
       || field_condition[row][col].second > 2)
        there_is_to_execute = true;
}

bool game_field::is_settable_by_row
        (size_t row, size_t col, const content_t &type) const {

    return
            !col ||
            field[row][col - 1]->is_empty() ||
            (field[row][col - 1]->get_content()->get_content_type() != type) ||
            (field_condition[row][col - 1].second < 2);
}

bool game_field::is_settable_by_col
        (size_t row, size_t col, const content_t &type) const {

    return
            !row ||
            field[row - 1][col]->is_empty() ||
            field[row - 1][col]->get_content()->get_content_type() != type ||
            field_condition[row - 1][col].first < 2;
}

void game_field::swap_reaction(const direction_t &dir) {
    if(clicked_position == none_position || dir == direction_t::NONE)
        return;

    if(!field[clicked_position.first][clicked_position.second]->is_swappable() ||
       !get_cell_by_direction(dir)->is_swappable())
        return;

    execution_set_t execution_set;
    execution_set.insert(
            std::make_shared <swap_action> (
                    field[clicked_position.first][clicked_position.second],
                    get_cell_by_direction(dir)
            )
    );
    execution_q.push(execution_set);
    update(0);
    moves++;

    update_field_condition();

    if(!there_is_to_execute) {
        moves--;
        execution_set.clear();
        execution_set.insert(
                std::make_shared <swap_action> (
                        field[clicked_position.first][clicked_position.second],
                        get_cell_by_direction(dir)
                )
        );
        execution_q.push(execution_set);
        update_field_condition();
    }

    clicked_position = none_position;
}

direction_t game_field::get_dir_of_focused_neighbor() const {
    if(clicked_position == none_position)
        return direction_t::NONE;

    if((clicked_position.second > 0)
       && field[clicked_position.first][clicked_position.second - 1]
                         ->is_focus_on()){
        return direction_t::LEFT;
    }

    if((clicked_position.first > 0)
       && field[clicked_position.first - 1][clicked_position.second]
                         ->is_focus_on())
        return direction_t::UP;

    if((clicked_position.second < (int) get_cols() - 1)
       && field[clicked_position.first][clicked_position.second + 1]
                         ->is_focus_on())
        return direction_t::RIGHT;

    if((clicked_position.first < (int) get_rows() - 1)
       && field[clicked_position.first + 1][clicked_position.second]
                         ->is_focus_on())
        return direction_t::DOWN;

    return direction_t::NONE;
}

shared_ptr <game_cell>& game_field::get_cell_by_direction(const direction_t &dir) {
    if(get_dir_of_focused_neighbor() == direction_t::NONE) {
        throw (std::range_error("in game_field::get_cell_by_direction - can not get that element"));
    }

    return field
        [clicked_position.first + direction::dir_y[static_cast<size_t> (dir)]]
        [clicked_position.second + direction::dir_x[static_cast<size_t> (dir)]];
}


void game_field::construct_field(const level &lvl) {
    execution_set_t creation_set;

    for(size_t i = 0; i < get_rows(); i++) {
        for(size_t j = 0; j < get_cols(); j++) {
            field[i][j] = std::make_shared <game_cell> ();
            field[i][j]->set_window(window);
            field[i][j]->set_type(lvl.get_cell_t(i, j));
            designer->wear_cell(field[i][j]);
            if(!field[i][j]->is_free() && field[i][j]->is_existing())
                goal++;

            field[i][j]->set_position(Vector2f(
                    area::position.x + j * field[i][j]->get_size().x,
                    area::position.y + i * field[i][j]->get_size().y
            ));

            if(!field[i][j]->is_settable())
                continue;

            shared_ptr <cell_content> new_content
                    = generate_dressed_content_at_first(i, j);

            creation_set.insert(
                    std::make_shared <create_action> (
                            field[i][j], new_content)
            );
            update_cell_condition(i, j);
        }
    }

    execution_q.push(creation_set);
}

shared_ptr <cell_content> game_field::generate_dressed_content
        (const content_t &type, const bonus_t &bonus) {

    shared_ptr <cell_content> new_content =
            std::make_shared <cell_content> (type, bonus);
    designer->wear_content(new_content);

    return new_content;
}

shared_ptr <cell_content> game_field::generate_dressed_content_at_first
        (size_t row, size_t col) {

    content_t temp = random_element::get_content_t();

    while (!is_settable_by_col(row, col, temp)
           || !is_settable_by_row(row, col, temp))
        temp = random_element::get_content_t();

    return generate_dressed_content(temp);
}

game_field::pair_ii game_field::get_position_in_focus() const {
    for(size_t row = 0; row < get_rows(); row++) {
        for(size_t col = 0; col < get_cols(); col++) {
            if(field[row][col]->is_focus_on())
                return {row, col};
        }
    }

    return none_position;
}

void game_field::calc_new_bonuses() {
    bonuses.clear();

    for(uint i = 0; i < get_rows(); i++)
        for(uint j = 0; j < get_cols(); j++) {
            if(field_condition[i][j].second != 1)
                continue;
            if(!field[i][j] || field[i][j]->is_empty())
                continue;

            pair <bonus_t, pair_ii> bonus = recognize_bonus(i, j);
            if(bonus.first != bonus_t::NONE)
                bonuses.insert(
                        {bonus.second, {field[i][j]->get_content()->get_content_type()
                                , bonus.first}});
        }
}


pair<bonus_t, game_field::pair_ii> game_field::recognize_bonus
        (size_t row, size_t col) const {

    uint h_number = 0;
    uint v_number = 0;
    pair_ii center = {row, col};

    if(field_condition[row][col].second != 1)
        return {bonus_t::NONE, center};

    for(uint cur_col = col; ; cur_col++) {
        if(cur_col >= get_cols() ||
           field[row][cur_col]->is_empty() ||
           !(*field[row][cur_col] == *field[row][col])) {
            break;
        }
        h_number++;
        uint cur_v_number = recognize_bonus_by_col(row, cur_col);
        if(cur_v_number > v_number) {
            v_number = cur_v_number;
            center = {row, cur_col};
        }
    }

    bonus_t bonus = recognize_bonus_by_res(h_number, v_number);
    if(bonus_t::V == bonus &&
            field_condition[center.first][center.second].first != 1)
        bonus = bonus_t ::NONE;

    return {bonus, center};
}

uint game_field::recognize_bonus_by_col(size_t row, size_t col) const {
    uint v_number = field_condition[row][col].first;
    for(uint cur_row = row + 1; ; cur_row++) {
        if(cur_row >= get_rows() ||
           field_condition[cur_row][col].first <=
           field_condition[row][col].first)
        {
            break;
        }

        v_number++;
    }
    return v_number;
}

bonus_t game_field::recognize_bonus_by_res(uint h_number, uint v_number) const {
    if(h_number >= 4 && v_number <= 2)
        return bonus_t::H;
    if(v_number >= 4 && h_number <= 2)
        return bonus_t::V;
    if(v_number == 3 && h_number == 3)
        return bonus_t::BOMB;
    if(v_number == 3 && h_number == 4 || v_number == 4 && h_number == 3)
        return bonus_t::MAX_BOMB;
    if(v_number >= 4 && h_number >= 4 || v_number == 3 && h_number >= 5 ||
            v_number >= 5 && h_number == 3)
        return bonus_t::CROSS;

    return bonus_t::NONE;
}



void game_field::set_bonus_content(size_t row, size_t col,
                                  execution_set_t &execution_set) {
    if(bonuses.empty())
        return;

    shared_ptr <cell_content> new_content =
            generate_dressed_content(bonuses[{row, col}].first, bonuses[{row, col}].second);
    execution_set.insert
            (std::make_shared <create_action> (field[row][col],
                                                   new_content));
}

void game_field::activate_bonus(set <pair_ii> &bonus_set, const pair_ii &coordinate) {
    if(!field[coordinate.first][coordinate.second] ||
            field[coordinate.first][coordinate.second]->is_empty())
        return;

    switch (field[coordinate.first][coordinate.second]->get_content()
            ->get_bonus()) {
        case bonus_t::NONE :
            return;

        case bonus_t::V :
            activate_V_bonus(bonus_set, coordinate);
            return;

        case bonus_t::H :
            activate_H_bonus(bonus_set, coordinate);
            return;

        case bonus_t::BOMB :
            activate_BOMB_bonus(bonus_set, coordinate);
            return;

        case bonus_t::MAX_BOMB :
            activate_MAX_BOMB_bonus(bonus_set, coordinate);
            return;

        case bonus_t::CROSS :
            activate_CROSS_bonus(bonus_set, coordinate);
            return;
    }
}

void game_field::activate_bonus_cell(set <pair_ii> &bonus_set,
                                     const pair_ii &coordinate) {
    if(!field[coordinate.first][coordinate.second])
        return;

    downdrade_s.insert(coordinate);
    clearice_s.insert(coordinate);
    opencage_s.insert(coordinate);

    if(field[coordinate.first][coordinate.second]->is_empty())
        return;

    if(bonus_set.find({coordinate.first, coordinate.second}) ==
            bonus_set.end()) {
        bonus_set.insert({coordinate.first, coordinate.second});
        activate_bonus(bonus_set, {coordinate.first, coordinate.second});
    }
    else
        bonus_set.insert({coordinate.first, coordinate.second});
}

void game_field::activate_V_bonus(set <pair_ii> &bonus_set,
                                const pair_ii &coordinate) {
    for(uint row = 0; row < get_rows(); row++) {
        activate_bonus_cell(bonus_set, {row, coordinate.second});
    }
}

void game_field::activate_H_bonus(set <pair_ii> &bonus_set,
                                  const pair_ii &coordinate) {
    for(uint col = 0; col < get_cols(); col++) {
        activate_bonus_cell(bonus_set, {coordinate.first, col});
    }
}

void game_field::activate_BOMB_bonus(set <pair_ii> &bonus_set,
                                     const pair_ii &coordinate) {
    for(uint row = std::max(0, coordinate.first - 1);
        row <= std::min(coordinate.first + 1,
                        static_cast <int> (get_rows()) - 1); row++) {

        for(uint col = std::max(0, coordinate.second - 1);
                col <= std::min(coordinate.second + 1,
                                static_cast <int> (get_cols()) - 1); col++) {
            activate_bonus_cell(bonus_set, {row, col});
        }
    }
}

void game_field::activate_MAX_BOMB_bonus(set <pair_ii> &bonus_set,
                                         const pair_ii &coordinate) {
    activate_BOMB_bonus(bonus_set, coordinate);

    for (uint i = 1; i <= direction::size; i++) {
        int cur_row = coordinate.first + direction::dir_x[i] * 2;
        int cur_col = coordinate.second + direction::dir_y[i] * 2;

        if(cur_row >= 0 && cur_row < static_cast <int> (get_rows()) &&
                cur_col >= 0 && cur_col < static_cast <int> (get_cols())) {
            activate_bonus_cell(bonus_set, {cur_row, cur_col});
        }
    }
}

void game_field::activate_CROSS_bonus(set <pair_ii> &bonus_set,
                                      const pair_ii &coordinate) {
    activate_H_bonus(bonus_set, coordinate);
    activate_V_bonus(bonus_set, coordinate);
}

void game_field::create_execution_set() {
    execution_set_t execution_set;
    set <pair_ii> execution_coordinates;

    create_execution_coordinates(execution_coordinates);

    for(auto &it : execution_coordinates) {
        execution_set.insert
                (std::make_shared <delete_action> (field[it.first][it.second]));
    }

    if(!execution_set.empty())
        execution_q.push(execution_set);
}

void game_field::create_execution_coordinates(set<pair_ii> &execution_coordinates) {
    for(size_t row = 0; row < get_rows(); row++) {
        for(size_t col = 0; col < get_cols(); col++) {

            if(field_condition[row][col].first >= 3) {
                push_same_line_to_execution_set
                        (row, col, orientation_t::V, execution_coordinates);
            }

            if(field_condition[row][col].second >= 3) {
                push_same_line_to_execution_set
                        (row, col, orientation_t::H, execution_coordinates);
            }
        }
    }

    set <pair_ii> execution_bonus_coordinates;
    for(auto &it : execution_coordinates) {
        activate_bonus(execution_bonus_coordinates, it);
    }
    for(auto &it : execution_bonus_coordinates) {
        if(execution_coordinates.find(it) != execution_coordinates.end())
            continue;

        execution_coordinates.insert(it);
    }

}

void game_field::push_same_line_to_execution_set(size_t row, size_t col,
                                                 const orientation_t &o,
                                                 set <pair_ii> &execution_coordinates) {
    execution_coordinates.insert({row, col});

    set_change_cell(row, col);

    if(o == orientation_t::H) {
        if(col == 0)
            return;

        for(size_t cur_col = col - 1;
            field_condition[row][cur_col].second + 1 == field_condition[row][cur_col + 1].second;
            cur_col--) {

            if(field[row][cur_col]->is_empty()) return;
            execution_coordinates.insert({row, cur_col});
            set_change_cell(row, cur_col);
            if(cur_col == 0) return;
        }
    }

    if(o == orientation_t::V) {
        if(row == 0)
            return;

        for(size_t cur_row = row - 1;
            field_condition[cur_row][col].first + 1 == field_condition[cur_row + 1][col].first;
            cur_row--) {

            if(field[cur_row][col]->is_empty()) return;
            execution_coordinates.insert({cur_row, col});
            set_change_cell(cur_row, col);
            if(cur_row == 0) return;
        }
    }
}

void game_field::create_creation_set() {
    execution_set_t execution_set;

    for(auto &it : bonuses) {
        set_bonus_content(it.first.first, it.first.second, execution_set);
    }

    for(size_t row = get_rows() - 1; ; row--) {
        for(size_t col = get_cols() - 1; ; col--) {
            if(field[row][col] && field[row][col]->is_settable() &&
                    field[row][col]->is_empty()) {
                if(bonuses.find({row, col}) == bonuses.end())
                    set_upper_content(row, col, execution_set);
            }
            if(!col) break;
        }
        if(!row) break;
    }

    if(execution_set.empty())
        return;

    execution_q.push(execution_set);
    bonuses.clear();
}

void game_field::set_upper_content(size_t row, size_t col, execution_set_t &execution_set) {
    for(int i = row - 1; i >= 0; i--) {
        if(field[i][col] && !field[i][col]->is_empty()) {
            create_distance[col] = field[row][col]->get_position().y -
                                   field[i][col]->get_position().y;

            execution_set.insert(
                    std::make_shared <create_action> (field[row][col], field[i][col]->get_content(),
                                                      create_distance[col]));
            field[i][col]->clear_content();
            return;
        }
    }

    shared_ptr <cell_content> new_content =
            generate_dressed_content(random_element::get_content_t());
    if(create_distance[col] == 0)
        create_distance[col] = field[row][col]->get_position().y - area::position.y;
    execution_set.insert
            (std::make_shared <create_action> (field[row][col], new_content, create_distance[col]));
}

void game_field::set_change_cell(uint row, uint col) {
    set_downgrade_around(row, col);
    clearice_s.insert({row, col});
    opencage_s.insert({row, col});
}

void game_field::set_downgrade_around(uint row, uint col) {
    if(col)
        downdrade_s.insert({row, col - 1});
    if(row)
        downdrade_s.insert({row - 1, col});
    if(col < get_cols() - 1)
        downdrade_s.insert({row, col + 1});
    if(row < get_rows() - 1)
        downdrade_s.insert({row + 1, col});
}

void game_field::complete_changes() {
    for(auto &it : downdrade_s) {
        field[it.first][it.second]->downgrade();
        calculate_change(it.first, it.second);
    }
    for(auto &it : clearice_s) {
        field[it.first][it.second]->clearice();
        calculate_change(it.first, it.second);
    }
    for(auto &it : opencage_s) {
        field[it.first][it.second]->open_cage();
        calculate_change(it.first, it.second);
    }
    downdrade_s.clear();
    clearice_s.clear();
    opencage_s.clear();
}

void game_field::calculate_change(uint row, uint col) {

    if(field[row][col]->need_to_redesign()){
        designer->wear_cell(field[row][col]);
        if(field[row][col]->is_free())
            goal--;
    }
}
