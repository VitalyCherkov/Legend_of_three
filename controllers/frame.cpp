//
// Created by vcherkov on 08.05.17.
//

#include "frame.h"

frame::frame(const shared_ptr<sf::RenderWindow> &window,
           uint cols, uint rows, const sf::String &data) :
        i_controller(window),
        cols(cols),
        rows(rows),
        data()
{
    area::position = sf::Vector2f(0, 0);
    sf::Vector2f point_size = global_parameters::get_point_size();
    area::size = sf::Vector2f(
            point_size.x * static_cast <float> (cols),
            point_size.y * static_cast <float> (rows)
    );
    set_data(data);
}

void frame::set_data(const sf::String &new_data) {
    data.setString(new_data);
}

void frame::set_style(const sf::Font &font, const sf::Color &color,
                      const uint size) {
    data.setFont(font);
    data.setFillColor(color);
    data.setCharacterSize(size);

    float width = data.getGlobalBounds().width;
    float height = data.getGlobalBounds().height;
    width = (get_size().x - width) / 2.0;
    height = (get_size().y - height) / 2.0;
    auto pos = get_position();
    pos.x += width;
    pos.y += height;

    data.setPosition(pos);
}

void frame::set_text(const sf::Text &new_text) {
    data = new_text;
}

void frame::set_texture(const shared_ptr<sf::Texture> &texture) {
    if(!texture)
        throw std::invalid_argument("frame::set_texture() - texture cannot be empty");

    this->texture = texture;

    apply_texture();
}

unsigned int frame::get_rows() const {
    return rows;
}

unsigned int frame::get_cols() const {
    return cols;
}

controller_t frame::get_type() const {
    return controller_t::FRAME;
}

void frame::set_position(const sf::Vector2f &position) {
    area::position = position;
    sprite.setPosition(position);

    float width = data.getGlobalBounds().width;
    float height = data.getGlobalBounds().height;
    width = (get_size().x - width) / 2.0;
    height = (get_size().y - height) / 2.0;
    auto pos = get_position();
    pos.x += width;
    pos.y += height;

    data.setPosition(pos);
}

void frame::draw() {
    window->draw(sprite);
    window->draw(data);
}

void frame::apply_texture() {
    if(!texture)
        throw std::logic_error("frame:: can not apply empty texture");

    texture->setRepeated(true);
    sprite.setTexture(*texture, true);
}