#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <btBulletDynamicsCommon.h>

class Entity {
public:
    Entity();
    virtual ~Entity() = 0;

    virtual void onUpdate(double dt);
    virtual void onDraw(sf::RenderTarget& target);
    virtual void onHandleEvent(sf::Event& event);

    glm::vec2 m_position = glm::vec2(0, 0);
    glm::vec2 m_scale = glm::vec2(1, 1);
    float m_rotation = 0.f;

    // physics stuff
    btCollisionShape* m_physicsShape;
    btRigidBody* m_physicsBody;
};

#endif
