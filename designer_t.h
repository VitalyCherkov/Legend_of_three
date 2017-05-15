//
// Created by vcherkov on 28.04.17.
//

#ifndef LEGEND_OF_THREE_DESIGNER_H
#define LEGEND_OF_THREE_DESIGNER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "field/game_cell.h"
#include "field/cell_content.h"
#include "controllers/all_controllers.h"

class designer_t {
    typedef std::pair <int, int> pair_ii;
    typedef std::shared_ptr <sf::Texture> pTexture;

public:
    designer_t(const std::string &field_texture,
               const std::string &main_texture,
               const std::string &main_filename);

    void set_skinpack(const std::string &field_filename,
                      const std::string &content_filename);

    void wear_cell(std::shared_ptr <game_cell> &cell) const;
    void wear_content(std::shared_ptr <cell_content> &content) const;
    void wear_controller(std::shared_ptr <i_controller> &controller);

    void set_unusual(bool unusual);
private:
    void wear_bonus(std::shared_ptr <cell_content> &content) const;

    void wear_slider(std::shared_ptr <slider> &controller);
    void wear_switcher(std::shared_ptr <switcher> &controller);
    void wear_button(std::shared_ptr <button> &controller);
    void wear_frame(std::shared_ptr <frame> &controller);

    pTexture get_subtexture(const sf::Vector2i &position, const sf::Vector2i size);

    sf::Vector2i get_postion_by_size(const pair_ii &local_size) const;


    const sf::Color background_color = sf::Color(92, 151, 133);

    bool unusual;

    sf::Vector2i size;
    sf::Image image;
    sf::Image content_img;
    sf::Texture content_texture;
    sf::Texture texture;
    map <pair_ii, pTexture>
            controller_textures;

    sf::Font font;
    sf::Color font_color;

    sf::Image main_image;
    sf::Texture main_texture;

};

#endif //LEGEND_OF_THREE_DESIGNER_H
