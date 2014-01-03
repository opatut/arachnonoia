#include "Entity.hpp"

#include "EntityMotionState.hpp"

Entity::~Entity() {
    delete m_physicsBody;
    delete m_motionState;
    delete m_physicsShape;
}

void Entity::onUpdate(double dt) {}
void Entity::onDraw(sf::RenderTarget& target) {}
void Entity::onHandleEvent(sf::Event& event) {}

glm::vec2 Entity::position() const
{
    return m_position;
}

void Entity::setPosition(const glm::vec2 &new_position)
{
    m_position = new_position;
}

glm::vec2 Entity::scale() const
{
    return m_scale;
}

void Entity::setScale(const glm::vec2 &new_scale)
{
    m_scale = new_scale;
}

float Entity::rotation() const
{
    return m_rotation;
}

void Entity::setRotation(float new_rotation)
{
    m_rotation = new_rotation;
}

btCollisionShape *Entity::physicsShape() const
{
    return m_physicsShape;
}

void Entity::setPhysicsShape(btCollisionShape *new_physicsShape)
{
    m_physicsShape = new_physicsShape;
}

btRigidBody *Entity::physicsBody() const
{
    return m_physicsBody;
}

void Entity::setPhysicsBody(btRigidBody *new_physicsBody)
{
    m_physicsBody = new_physicsBody;
}

EntityMotionState *Entity::motionState() const
{
    return m_motionState;
}

void Entity::setMotionState(EntityMotionState *new_motionState)
{
    m_motionState = new_motionState;
}
