#include "Entity.hpp"

Entity::Entity() {
    // We check whether we need to initialize physics by checking these members against
    // nullptr, so let's set them to that so that we may check again later.
    m_physicsShape = nullptr;
    m_physicsBody = nullptr;
}

Entity::~Entity() {
    delete m_physicsBody;
    delete m_physicsShape;
}

void Entity::onUpdate(double dt) {}
void Entity::onDraw(sf::RenderTarget& target) {}
void Entity::onHandleEvent(sf::Event& event) {}
