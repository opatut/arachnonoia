#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <btBulletDynamicsCommon.h>

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = 0;

    virtual void onUpdate(double dt);
    virtual void onDraw(sf::RenderTarget& target);
    virtual void onHandleEvent(sf::Event& event);

    glm::vec2 m_position = glm::vec2(0, 0);
    glm::vec2 m_scale = glm::vec2(1, 1);
    float m_rotation = 0.f;

    // physics stuff
    // We check whether we need to initialize physics by checking these members against
    // nullptr, so let's set them to that so that we may check again later.
    btCollisionShape* m_physicsShape = nullptr;
    btRigidBody* m_physicsBody = nullptr;
};

#endif
