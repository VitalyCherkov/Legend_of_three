//
// Created by vcherkov on 10.05.17.
//

#ifndef LEGEND_OF_THREE_LEVEL_H
#define LEGEND_OF_THREE_LEVEL_H

#include "tinyxml2.h"
#include "utiles.h"

class level {
public:
    level();
    bool load_from_file(const std::string &filename);

    const cell_t &get_cell_t(uint row, uint col) const;
    uint get_cols () const;
    uint get_rows() const;
    uint get_level_number() const;
    uint get_moves() const;
private:
    uint loaded;
    uint level_number;
    uint moves;

    bool load_level_info(const tinyxml2::XMLElement* level_info);
    bool load_size(const tinyxml2::XMLElement* map);
    bool load_object_group(tinyxml2::XMLElement* group);
    bool load_object(tinyxml2::XMLElement* object);

    std::vector < std::vector <cell_t> > _level;
};


#endif //LEGEND_OF_THREE_LEVEL_H
