#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Player : public Entity {
public:
    Player();

    std::string getTypeName();

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State *state);
    bool onCollide(Entity* other);

private:
    sf::Sprite m_sprite;
    float m_scale_y;
};

#endif
