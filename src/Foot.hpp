#ifndef FOOT_HPP
#define FOOT_HPP

#include "Entity.hpp"

class Foot : public Entity {
public:
    Foot(Entity* player);

    void onDraw(sf::RenderTarget &target);
    void onAdd(State* state);

private:
    Entity* m_player;
    btCollisionShape* m_leftFootShape;
    btCollisionShape* m_rightFootShape;
};

#endif // FOOT_HPP
