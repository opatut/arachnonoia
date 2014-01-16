#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <btBulletDynamicsCommon.h>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "CerealGLM.hpp"

class EntityMotionState;
class State;

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = 0;

    virtual void onUpdate(double dt);
    virtual void onDraw(sf::RenderTarget& target);
    virtual void onHandleEvent(sf::Event& event);
    virtual void onAdd(State *state);

    virtual glm::vec2 getSize();

    glm::vec2 position() const;
    void setPosition(const glm::vec2& pos);

    float rotation() const;
    void setRotation(float rot);

    void setPhysicsPosition(const glm::vec2& new_position);

    void setPhysicsRotation(float new_rotation);

    glm::vec2 scale() const;
    void setScale(const glm::vec2& new_scale);

    btScalar mass() const;
    void setMass(const btScalar &new_mass);

    btCollisionShape *physicsShape() const;
    void setPhysicsShape(btCollisionShape* new_physicsShape);

    EntityMotionState *motionState() const;
    void setMotionState(EntityMotionState* new_motionState);

    btRigidBody *physicsBody() const;
    void setPhysicsBody(btRigidBody *new_physicsBody);

    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("position", m_position));
        ar(cereal::make_nvp("scale",    m_scale));
        ar(cereal::make_nvp("rotation", m_rotation));
        ar(cereal::make_nvp("mass", m_mass));
    }

protected:
    glm::vec2 m_position = glm::vec2(0, 0);
    float m_rotation = 0.f;
    glm::vec2 m_scale = glm::vec2(0.01, 0.01);
    btScalar m_mass = 0.f;

    // We check whether we need to initialize physics by checking these members against
    // nullptr, so let's set them to that so that we may check again later.
    btCollisionShape* m_physicsShape = nullptr;
    EntityMotionState* m_motionState = nullptr;
    btRigidBody* m_physicsBody = nullptr;
};

#endif
