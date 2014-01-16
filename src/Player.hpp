#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Foot.hpp"

class Player : public Entity {
public:
    Player();

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State *state);

private:
    sf::Sprite m_sprite;

    // The feet go from left to right
    std::shared_ptr<Foot> m_feet[5];
};

#endif
