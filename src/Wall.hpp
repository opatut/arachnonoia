#ifndef WALL_HPP
#define WALL_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Wall : public Entity {
public:
    static const std::string types[2];
    Wall();

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;

private:
    std::string m_type;
    sf::Sprite m_sprite;

};

#endif
