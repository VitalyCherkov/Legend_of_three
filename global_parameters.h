//
// Created by vcherkov on 07.05.17.
//

#ifndef LEGEND_OF_THREE_GLOBAL_PARAMETERS_H
#define LEGEND_OF_THREE_GLOBAL_PARAMETERS_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "tinyxml2.h"
#include "utiles.h"

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::vector;
using std::map;
using std::pair;

class global_parameters {
public:
    struct bounds{
        pair <uint, uint> size;
        pair <uint, uint> pos;
    };

    static bool load_global_parameters(string xml_filename);

    static string &get_main_texture();
    static string &get_field_texture();
    static string &get_skin_texture();
    static uint get_num_of_levels();
    static string &get_level_file(uint index);

    static sf::Vector2f &get_point_size();
    static sf::Vector2f &get_cell_size();

    static bounds get_controller_bounds(const string &name,
                                        const string &type = "");
    static sf::Vector2i &get_cell_bounds(const cell_t &cell);
    static sf::Vector2i &get_content_bounds(const content_t &cnt);
    static sf::Vector2i &get_bonus_bounds(const bonus_t &bonus);

    static sf::Vector2i &get_delta();

    static cell_t convert_gid(uint gid);
    static content_t convert_cid(uint cid);
    static bonus_t convert_bid(uint bid);
private:

    // ------------------------------------------------------
    // Loading !
    // ------------------------------------------------------

    static bool load_resources
            (const tinyxml2::XMLElement* files);
    static bool load_texture_names
            (const tinyxml2::XMLElement* textures);
    static bool load_levels
            (const tinyxml2::XMLElement* levels);
    static bool load_level_names
            (const tinyxml2::XMLElement* levels);

    // ------------------------------------------------------
    // Main texture loading
    // ------------------------------------------------------

    static bool load_main_texture
            (const tinyxml2::XMLElement* main_texture);
    static bool load_controller_point
            (const tinyxml2::XMLElement *point);
    static bool load_controllers
            (const tinyxml2::XMLElement* controllers);
    static void load_controller
            (const tinyxml2::XMLElement* controller);

    // ------------------------------------------------------
    // Field texture loading
    // ------------------------------------------------------

    static bool load_field_texture
            (const tinyxml2::XMLElement* field_texture);
    static bool load_field_point
            (const tinyxml2::XMLElement* point);
    static bool load_field_objects
            (const tinyxml2::XMLElement* objects);
    static bool load_field_object
            (const tinyxml2::XMLElement* object);

    // ------------------------------------------------------
    // Content loading
    // ------------------------------------------------------

    static bool load_content_texture
            (const tinyxml2::XMLElement* field_texture);
    static bool load_field_delta
            (const tinyxml2::XMLElement* delta);
    static bool load_content
            (const tinyxml2::XMLElement* content);

    // ------------------------------------------------------
    // Bonus loading
    // ------------------------------------------------------

    static bool load_bonus
            (const tinyxml2::XMLElement* content);

    // ------------------------------------------------------
    // Other loading
    // ------------------------------------------------------

    template <typename T>
    static bool load_vec2f(const std::string &attr1,
                                   const std::string &attr2,
                                   const tinyxml2::XMLElement * from,
                                   sf::Vector2 <T> &to);



    static vector <string> level_files;
    static string main_texture_file;
    static string field_texture_file;
    static string skin_texture_file;

    static map <content_t, sf::Vector2i> content;
    static map <pair <string, string>, bounds> controllers;
    static map <cell_t, sf::Vector2i > cells;
    static map <bonus_t, sf::Vector2i > bonuses;
    static sf::Vector2f main_point;
    static sf::Vector2f field_point;
    static sf::Vector2i field_delta;

    global_parameters();
};


#endif //LEGEND_OF_THREE_GLOBAL_PARAMETERS_H
