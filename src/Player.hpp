#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Player : public Entity {
public:
    Player();

    void onUpdate(double dt);
    void onDraw(sf::RenderTarget& target);

private:
    sf::Sprite m_sprite;

};

#endif
