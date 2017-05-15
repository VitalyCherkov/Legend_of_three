//
// Created by vcherkov on 07.05.17.
//
#include <sstream>
#include <iostream>
#include "global_parameters.h"
using namespace tinyxml2;

sf::Vector2f global_parameters::main_point(0, 0);
map <pair <string, string>, global_parameters::bounds>
        global_parameters::controllers =
        map <pair <string, string>, global_parameters::bounds>();
map <content_t, sf::Vector2i > global_parameters::content =
        map <content_t, sf::Vector2i > ();
map <cell_t, sf::Vector2i > global_parameters::cells =
        map <cell_t, sf::Vector2i > ();
map <bonus_t, sf::Vector2i > global_parameters::bonuses =
        map <bonus_t, sf::Vector2i > ();

sf::Vector2f global_parameters::field_point(0, 0);
sf::Vector2i global_parameters::field_delta(0, 0);
vector <string> global_parameters::level_files = vector <string> ();
string global_parameters::main_texture_file = string();
string global_parameters::field_texture_file = string();
string global_parameters::skin_texture_file = string();


bool global_parameters::load_global_parameters(string xml_filename) {
    XMLDocument document;

    if(document.LoadFile(xml_filename.c_str()) != XML_SUCCESS)
        return false;

    XMLElement* global = document.FirstChildElement("global_parameters");

    bool loaded1 =
            load_main_texture(global->FirstChildElement("main_texture"));
    bool loaded2 =
            load_field_texture(global->FirstChildElement("field_texture"));
    bool loaded3 =
            load_content_texture(global->FirstChildElement("content_texture"));
    bool loaded4 =
            load_resources(global->FirstChildElement("files"));

    return loaded1 && loaded2 && loaded3 && loaded4;
}

string &global_parameters::get_main_texture() {
    return main_texture_file;
}

string &global_parameters::get_field_texture() {
    return field_texture_file;
}

string &global_parameters::get_skin_texture() {
    return skin_texture_file;
}

uint global_parameters::get_num_of_levels() {
    return level_files.size();
}

string &global_parameters::get_level_file(uint index) {
    return index < level_files.size() ? level_files[index] : level_files.back();
}

sf::Vector2f& global_parameters::get_point_size() {
    return main_point;
}

sf::Vector2f& global_parameters::get_cell_size() {
    return field_point;
}

global_parameters::bounds global_parameters::get_controller_bounds
        (const string &name, const string &type) {

    if(controllers.find({name, type}) == controllers.end())
        throw std::invalid_argument(
                "global_parameters::get_controller_bounds - "
                        "cannot find controller");

    return controllers.at({name, type});
}

sf::Vector2i &global_parameters::get_cell_bounds(const cell_t &cell) {
    return cells[cell];
}

sf::Vector2i &global_parameters::get_content_bounds
        (const content_t &cnt) {
    return content[cnt];
}

sf::Vector2i &global_parameters::get_bonus_bounds
        (const bonus_t &bonus) {
    return bonuses[bonus];
}

sf::Vector2i& global_parameters::get_delta() {
    return field_delta;
}

cell_t global_parameters::convert_gid(uint gid) {
    switch (gid) {
        case 1:
            return cell_t ::INPUT;
        case 2:
            return cell_t ::NONE;
        case 3:
            return cell_t ::FREE;
        case 9:
            return cell_t ::ICE1;
        case 4:
            return cell_t ::BOX1;
        case 10:
            return cell_t ::BOX2;
        case 16:
            return cell_t ::BOX3;
        case 22:
            return cell_t ::BOX4;
        case 5:
            return cell_t ::CAGE;
        default:
            return cell_t ::FREE;
    }
}

content_t global_parameters::convert_cid(uint cid) {
    return static_cast <content_t> (cid);
}

bonus_t global_parameters::convert_bid(uint bid) {
    return static_cast <bonus_t> (bid);
}

bool global_parameters::load_resources(const XMLElement *files) {
    if(!files)
        return false;

    bool res1 = load_texture_names(files);
    bool res2 = load_levels(files);
    return res1 && res2;
}

bool global_parameters::load_texture_names(const XMLElement* textures) {
    try {
        main_texture_file = textures->FirstChildElement("main_texture")->GetText();
        field_texture_file = textures->FirstChildElement("field_texture")->GetText();
        skin_texture_file = textures->FirstChildElement("skin_texture")->GetText();
        return true;
    }
    catch(...) {
        return false;
    }
}

bool global_parameters::load_levels(const XMLElement* levels) {
    if(!levels)
        return false;

    const XMLElement* level_name = levels->FirstChildElement("level");
    while(level_name) {
        level_files.push_back(level_name->GetText());
        level_name = level_name->NextSiblingElement("level");
    }
    return true;
}

bool global_parameters::load_main_texture(const XMLElement* main_texture) {
    if(!main_texture)
        return false;

    load_controller_point(main_texture->FirstChildElement("point"));
    load_controllers(main_texture->FirstChildElement("controllers"));

    return true;
}

bool global_parameters::load_controller_point(const XMLElement *point) {

    if(!point)
        return false;

    std::stringstream strstr;
    strstr << point->Attribute("width") << " "
           << point->Attribute("height");

    float width = 0;
    float height = 0;

    strstr >> width >> height;
    main_point = sf::Vector2f(width, height);
    return true;
}

bool global_parameters::load_controllers
        (const XMLElement* controllers) {

    if(!controllers)
        return false;


    const XMLElement* controller =
            controllers->FirstChildElement("controller");

    while(controller) {
        load_controller(controller);

        controller = controller->NextSiblingElement("controller");
    }
    return true;
}

void global_parameters::load_controller(const XMLElement* controller) {

    if(!controller)
        return;

    string name = controller->Attribute("name");
    string type = controller->Attribute("type");

    std::stringstream strstr;

    strstr << controller->FirstChildElement("col_size")->GetText() << " "
           << controller->FirstChildElement("row_size")->GetText() << " "
           << controller->FirstChildElement("col_pos")->GetText() << " "
           << controller->FirstChildElement("row_pos")->GetText() << " ";

    bounds b;
    strstr >> b.size.first >> b.size.second
           >> b.pos.first >> b.pos.second;
    controllers.insert({{name, type}, b});
}

bool global_parameters::load_field_texture
        (const tinyxml2::XMLElement *field_texture) {

    if(!field_texture)
        return false;

    bool res1 = load_field_point
            (field_texture->FirstChildElement("point"));
    bool res2 = load_field_objects
            (field_texture->FirstChildElement("objects"));
    return res1 && res2;
}

bool global_parameters::load_field_point
        (const XMLElement *point) {
    return load_vec2f("width", "height", point, field_point);
}

bool global_parameters::load_field_objects
        (const XMLElement *objects) {
    if(!objects)
        return false;

    const XMLElement* object = objects->FirstChildElement("object");

    while(object) {
        if(!load_field_object(object))
            return false;
        object = object->NextSiblingElement("object");
    }
    return true;
}

bool global_parameters::load_field_object
        (const XMLElement *object) {
    if(!object)
        return false;

    std::stringstream stream;
    try {
        stream << object->FirstChildElement("gid")->GetText() << " "
               << object->FirstChildElement("row_pos")->GetText() << " "
               << object->FirstChildElement("col_pos")->GetText();

        uint gid = 0;
        uint row_pos = 0;
        uint col_pos = 0;
        stream >> gid >> row_pos >> col_pos;
        cells[convert_gid(gid)] = {col_pos, row_pos};
    }
    catch (...){
        return false;
    }
    return true;
}

bool global_parameters::load_content_texture
        (const tinyxml2::XMLElement *field_texture) {
    bool res1 =
            load_field_delta(field_texture->FirstChildElement("delta"));

    const XMLElement* content = field_texture->FirstChildElement("content");

    while(content) {
        if(!load_content(content))
            return false;
        content = content->NextSiblingElement("content");
    }

    const XMLElement* bonus = field_texture->FirstChildElement("bonus");
    while(bonus) {
        if(!load_bonus(bonus))
            return false;
        bonus = bonus->NextSiblingElement("bonus");
    }

    return true && res1;
}


bool global_parameters::load_field_delta
        (const XMLElement *delta) {
    return load_vec2f("bonus", "other", delta, field_delta);
}

bool global_parameters::load_content
        (const tinyxml2::XMLElement *content) {
    if(!content)
        return false;

    std::stringstream stream;
    try {
        stream << content->FirstChildElement("cid")->GetText() << " "
               << content->FirstChildElement("row_pos")->GetText() << " "
               << content->FirstChildElement("col_pos")->GetText();

        uint cid = 0;
        uint row_pos = 0;
        uint col_pos = 0;
        stream >> cid >> row_pos >> col_pos;
        global_parameters::content[convert_cid(cid)] =
                {col_pos, row_pos};
    }
    catch (...){
        return false;
    }


    return true;
}

bool global_parameters::load_bonus
        (const tinyxml2::XMLElement *content) {
    if(!content)
        return false;

    std::stringstream stream;
    try {
        stream << content->FirstChildElement("bid")->GetText() << " "
               << content->FirstChildElement("row_pos")->GetText() << " "
               << content->FirstChildElement("col_pos")->GetText();

        uint bid = 0;
        uint row_pos = 0;
        uint col_pos = 0;
        stream >> bid >> row_pos >> col_pos;
        global_parameters::bonuses[convert_bid(bid)] =
                {col_pos, row_pos};
    }
    catch (...){
        return false;
    }


    return true;
}

template <typename T>
bool global_parameters::load_vec2f(const std::string &attr1,
                                           const std::string &attr2,
                                           const tinyxml2::XMLElement* from,
                                           sf::Vector2 <T> &to) {
    if(!from)
        return false;

    std::stringstream stream;
    try {
        stream << from->Attribute(attr1.c_str()) << " "
               << from->Attribute(attr2.c_str());
        stream >> to.x >> to.y;
    }
    catch (...){
        return false;
    }
    return true;
}
