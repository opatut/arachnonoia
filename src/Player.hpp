#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Foot.hpp"

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

    std::shared_ptr<Foot> m_feet[4];
};

#endif
