#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Entity.hpp"
#include "Foot.hpp"

class Player : public Entity {
public:
    Player();

    std::string getTypeName() const override;

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State *state);
    bool onCollide(Entity* other, const EntityCollision& c);

    int getPairsLeft() const;

private:
    sf::Sprite m_sprite;
    btGhostObject* m_ghostObject;
    float m_scale_y = 0;
    std::vector<std::shared_ptr<Foot>> m_foregroundFeet;
};

#endif
