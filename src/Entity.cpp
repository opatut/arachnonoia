#include "Entity.hpp"

#include "EntityMotionState.hpp"

Entity::~Entity() {
    // TODO: Do this using shared_ptrs
    if(m_physicsBody)
        delete m_physicsBody;

    if(m_motionState)
        delete m_motionState;

    if(m_physicsShape)
        delete m_physicsShape;
}

void Entity::onUpdate(double dt) {}
void Entity::onDraw(sf::RenderTarget& target) {}
void Entity::onHandleEvent(sf::Event& event) {}
void Entity::onAdd() {}

glm::vec2 Entity::getSize() {
    return glm::vec2(1, 1);
}

glm::vec2 Entity::position() const
{
    return m_position;
}

void Entity::setPosition(const glm::vec2& position)
{
    m_position = position;
}

float Entity::rotation() const
{
    return m_rotation;
}

void Entity::setRotation(float rotation)
{
    m_rotation = rotation;
}

void Entity::setPhysicsPosition(const glm::vec2& new_position) {
    if(m_physicsBody) {
//        auto transform = m_physicsBody->getWorldTransform();
//        transform.setOrigin(btVector3(new_position.x, new_position.y, 0));
//        m_physicsBody->setWorldTransform(transform);

        auto transform = m_physicsBody->getCenterOfMassTransform();
        transform.setOrigin(btVector3(new_position.x, new_position.y, 0));
        m_physicsBody->setCenterOfMassTransform(transform);
    }

    m_position = new_position;
}

void Entity::setPhysicsRotation(float new_rotation) {
    if(m_physicsBody) {
        auto transform = m_physicsBody->getWorldTransform();
        auto rot = transform.getRotation();
        rot.setEuler(0, 0, new_rotation);
        transform.setRotation(rot);
    }

    m_rotation = new_rotation;
}

glm::vec2 Entity::scale() const {
    return m_scale;
}

void Entity::setScale(const glm::vec2& new_scale) {
    m_scale = new_scale;
}

btCollisionShape* Entity::physicsShape() const {
    return m_physicsShape;
}

void Entity::setPhysicsShape(btCollisionShape* new_physicsShape) {
    m_physicsShape = new_physicsShape;
}

btRigidBody *Entity::physicsBody() const {
    return m_physicsBody;
}

void Entity::setPhysicsBody(btRigidBody* new_physicsBody) {
    m_physicsBody = new_physicsBody;
}

btScalar Entity::mass() const
{
    return m_mass;
}

void Entity::setMass(const btScalar& new_mass)
{
    if(m_physicsBody) {
        btVector3 inertia(0, 0, 0);
        m_physicsShape->calculateLocalInertia(m_mass, inertia);
        m_physicsBody->setMassProps(m_mass, inertia);
    }

    m_mass = new_mass;
}

EntityMotionState* Entity::motionState() const {
    return m_motionState;
}

void Entity::setMotionState(EntityMotionState* new_motionState) {
    m_motionState = new_motionState;
}
