#ifndef WALL_HPP
#define WALL_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Wall : public Entity {
public:
    static const std::string types[2];
    Wall();

    void onUpdate(double dt);
    void onDraw(sf::RenderTarget& target);

private:
    std::string m_type;
    sf::Sprite m_sprite;

};

#endif
