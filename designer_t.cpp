//
// Created by vcherkov on 28.04.17.
//

#include "designer_t.h"
#include "global_parameters.h"
#include <utility>


designer_t::designer_t(const std::string &field_texture,
                       const std::string &content_texture,
                       const std::string &main_texture) :
    unusual(false),
    font(),
    font_color(0, 151, 245),
    size(global_parameters::get_cell_size())
{
    font.loadFromFile("ArialBlack.ttf");
    set_skinpack(field_texture, content_texture);
    main_image.loadFromFile(main_texture);
}

void designer_t::set_skinpack(const std::string &field_filename,
                              const std::string &content_filename) {
    image.loadFromFile(field_filename);
    texture.loadFromImage(image);
    content_img.loadFromFile(content_filename);
    content_texture.loadFromImage(content_img);
}

void designer_t::wear_cell(std::shared_ptr <game_cell> &cell) const {
    //cell.set_size(size);
    sf::Vector2i position =
        global_parameters::get_cell_bounds(cell->get_cell_type());
    position.x *= size.x;
    position.y *= size.y;

    cell->set_background(texture, sf::IntRect(position, size));
}

void designer_t::wear_content(std::shared_ptr<cell_content> &content) const {
    if(!content)
        return;

    sf::Vector2i position = global_parameters::get_content_bounds
            (content->get_content_type());
    position.x *= size.x;
    position.y *= size.y;

    if(!unusual)
        position.y += global_parameters::get_delta().y * size.y;

    content->set_texture(content_texture, sf::IntRect(position, size));
    if(content->get_bonus() != bonus_t::NONE)
        wear_bonus(content);

}

void designer_t::wear_bonus(std::shared_ptr<cell_content> &content) const {
    sf::Vector2i position = global_parameters::get_bonus_bounds
            (content->get_bonus());
    position.x *= size.x;
    position.y *= size.y;

    content->set_bonus_texture
            (content_texture, sf::IntRect(position, size));
}

void designer_t::wear_controller(std::shared_ptr <i_controller> &controller) {
    if(!controller)
        return;


    if(controller->get_type() == controller_t::SLIDER) {
        std::shared_ptr <slider> sldr =
                std::static_pointer_cast <slider>(controller);
        wear_slider(sldr);
    }

    if(controller->get_type() == controller_t::SWITCHER) {
        std::shared_ptr <switcher> swtchr =
                std::static_pointer_cast <switcher>(controller);
        wear_switcher(swtchr);
    }

    if(controller->get_type() == controller_t::BUTTON) {
        std::shared_ptr <button> btn =
                std::static_pointer_cast <button>(controller);
        wear_button(btn);
    }

    if(controller->get_type() == controller_t::FRAME) {
        std::shared_ptr <frame> frm =
                std::static_pointer_cast <frame>(controller);
        wear_frame(frm);
    }
}

void designer_t::set_unusual(bool unusual) {
    this->unusual = unusual;
}

void designer_t::wear_slider(std::shared_ptr<slider> &controller) {

    global_parameters::bounds line =
            global_parameters::get_controller_bounds("slider");
    sf::Vector2i line_size = get_postion_by_size(line.size);
    sf::Vector2i f_line_pos = get_postion_by_size(line.pos);
    line.pos.second += line.size.second;
    sf::Vector2i s_line_pos = get_postion_by_size(line.pos);

    global_parameters::bounds point =
            global_parameters::get_controller_bounds("point");
    sf::Vector2i point_size = get_postion_by_size(point.size);
    sf::Vector2i f_point_pos = get_postion_by_size(point.pos);
    point.pos.second += point.size.second;
    sf::Vector2i s_point_pos = get_postion_by_size(point.pos);

    controller->set_texture(
            { get_subtexture(f_line_pos, line_size),
              get_subtexture(s_line_pos, line_size), },
            { get_subtexture(f_point_pos, point_size),
              get_subtexture(s_point_pos, point_size) }
    );
}

void designer_t::wear_switcher(std::shared_ptr<switcher> &controller) {

    global_parameters::bounds on_bounds =
            global_parameters::get_controller_bounds("switcher", "on");

    global_parameters::bounds off_bounds =
            global_parameters::get_controller_bounds("switcher", "off");

    sf::Vector2i switcher_size = get_postion_by_size(on_bounds.size);

    sf::Vector2i f_on_pos = get_postion_by_size(on_bounds.pos);
    on_bounds.pos.second += on_bounds.size.second;
    sf::Vector2i s_on_pos = get_postion_by_size(on_bounds.pos);

    sf::Vector2i f_off_pos = get_postion_by_size(off_bounds.pos);
    off_bounds.pos.second += off_bounds.size.second;
    sf::Vector2i s_off_pos = get_postion_by_size(off_bounds.pos);

    controller->set_texture(
            {get_subtexture(f_on_pos, switcher_size),
             get_subtexture(s_on_pos, switcher_size)},
            {get_subtexture(f_off_pos, switcher_size),
             get_subtexture(s_off_pos, switcher_size)}
    );
}

void designer_t::wear_button(std::shared_ptr<button> &controller) {
    std::string str_type = convertation::to_string(
            controller->get_button_t());
    global_parameters::bounds button_bound =
            global_parameters::get_controller_bounds("button", str_type);

    sf::Vector2i button_size = get_postion_by_size(button_bound.size);
    sf::Vector2i first_position = get_postion_by_size(button_bound.pos);
    button_bound.pos.second += button_bound.size.second;
    sf::Vector2i second_position = get_postion_by_size(button_bound.pos);

    controller->set_texture({
                    get_subtexture
                        (first_position, button_size),
                    get_subtexture
                        (second_position, button_size)
    });

    controller->set_style(font, font_color, 14);
}

void designer_t::wear_frame(std::shared_ptr<frame> &controller) {
    global_parameters::bounds frame_bound =
            global_parameters::get_controller_bounds("frame");

    sf::Vector2i button_size = get_postion_by_size(frame_bound.size);
    sf::Vector2i position = get_postion_by_size(frame_bound.pos);

    controller->set_texture(get_subtexture(position, button_size));
    controller->set_style(font, font_color, 14);
}

designer_t::pTexture designer_t::get_subtexture(
        const sf::Vector2i &position, const sf::Vector2i size) {

    pair_ii index = {position.x, position.y};

    if(controller_textures.find(index) ==
            controller_textures.end()) {

        controller_textures[index] =
                std::make_shared <sf::Texture> ();
        controller_textures.at(index)->loadFromImage
                (main_image, sf::IntRect(position, size));
    }

    return controller_textures.at(index);
}

sf::Vector2i designer_t::get_postion_by_size(const pair_ii &local_size) const {
    return sf::Vector2i(global_parameters::get_point_size().x
                        * local_size.first,
                        global_parameters::get_point_size().y
                        * local_size.second);
}
