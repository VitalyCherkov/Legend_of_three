//
// Created by vcherkov on 10.05.17.
//

#include <sstream>
#include <iostream>
#include "level.h"
#include "global_parameters.h"

using namespace tinyxml2;

level::level() :
    loaded(0),
    level_number(0),
    moves(0)
{ }

bool level::load_from_file
        (const std::string &filename) {

    _level.clear();
    loaded = level_number = moves = 0;

    XMLDocument document;
    if(document.LoadFile(filename.c_str()) != XML_SUCCESS)
        return false;

    XMLElement *lvl = document.FirstChildElement("map");

    if(!lvl)
        return false;

    load_size(lvl);

    bool res1 = load_level_info(lvl->FirstChildElement("level_info"));
    bool res2 = load_object_group
            (lvl->FirstChildElement("objectgroup"));

    return res1 && res2;
}

uint level::get_cols() const {
    return _level.size() ? _level[0].size() : 0;
}

uint level::get_rows() const {
    return _level.size();
}

uint level::get_level_number() const {
    return level_number;
}

uint level::get_moves() const {
    return moves;
}

const cell_t& level::get_cell_t(uint row, uint col) const {
    return _level[row][col];
}

bool level::load_level_info(const tinyxml2::XMLElement* level_info) {
    if(!level_info)
        return false;

    std::stringstream stream;
    try {
        stream << level_info->Attribute("moves") << " "
               << level_info->Attribute("number");
        stream >> moves >> level_number;
    }
    catch (...) {
        return false;
    }
    return true;
}

bool level::load_size(const XMLElement* map) {
    if(!map)
        return false;

    std::stringstream stream;
    try {
        stream << map->Attribute("width") << " "
               << map->Attribute("height");
    }
    catch (...) {
        stream << 0 << " " << 0;
    }

    uint rows = 0;
    uint cols = 0;
    stream >> cols >> rows;

    _level.resize(rows);
    for(uint i = 0; i < rows; i++)
        _level[i].resize(cols);

    return true;
}

bool level::load_object_group(XMLElement* group) {

    XMLElement* object = group->FirstChildElement("object");
    while(object) {
        if(!load_object(object))
            return false;
        object = object->NextSiblingElement("object");
    }
    return true;
}

bool level::load_object(tinyxml2::XMLElement *object) {
    std::stringstream stream;

    try {
        stream << object->Attribute("gid") << " "
               << object->Attribute("x") << " "
               << object->Attribute("y") << " "
               << object->Attribute("width") << " "
               << object->Attribute("height");
    }
    catch (...){
        return false;
    }

    uint gid = 0;
    uint row = 0;
    uint col = 0;
    uint w = 0;
    uint h = 0;
    stream >> gid >> col >> row >> w >> h;

    if(!w || !h)
        return false;

    col /= w;
    row /= h;
    cell_t cell = global_parameters::convert_gid(gid);

    _level[row - 1][col] = cell;

    return true;
}





