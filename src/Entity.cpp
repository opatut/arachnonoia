#include "Entity.hpp"

#include "EntityMotionState.hpp"

Entity::Entity() 
    : m_freshman(true) {}

Entity::~Entity() {
    // TODO: Do this using shared_ptrs
    if(m_physicsBody)
        delete m_physicsBody;

    if(m_motionState)
        delete m_motionState;

    if(m_physicsShape)
        delete m_physicsShape;
}

void Entity::handleAddedToState(State* state) {
    m_state = state;
    m_lifeTime = 0;
    m_freshman = true;
    onAdd(state);
}

void Entity::handleUpdate(double dt) {
    m_lifeTime += dt;
    m_freshman = false;
    onUpdate(dt);
}

void Entity::handleDraw(sf::RenderTarget& target) {
    if(!m_freshman) {
        onDraw(target);
    }
}

void Entity::onUpdate(double dt) {}
void Entity::onDraw(sf::RenderTarget& target) {}
void Entity::onHandleEvent(sf::Event& event) {}
void Entity::onInitialize() {}
void Entity::onAdd(State* state) {}
void Entity::onRemove(State* state) {}
bool Entity::onCollide(Entity* other, const EntityCollision& c) {
    return false;
}

void Entity::setMetadata(int data) {}

glm::vec2 Entity::getSize() {
    return glm::vec2(1, 1);
}

glm::vec2 Entity::position() const
{
    return m_position;
}

void Entity::setPosition(const glm::vec2& pos)
{
    m_position = pos;
}

float Entity::rotation() const
{
    return m_rotation;
}

void Entity::setRotation(float rot)
{
    m_rotation = rot;
}

int Entity::zLevel() const {
    return m_zLevel;
}

void Entity::setZLevel(int z_level) {
    m_zLevel = z_level;
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
        auto& transform = m_physicsBody->getWorldTransform();
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

void Entity::kill() {
    m_deleted = true;
}

bool Entity::isDeleted() const {
    return m_deleted;
}
