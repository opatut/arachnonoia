#include "Entity.hpp"

Entity::~Entity() {
    delete m_physicsBody;
    delete m_physicsShape;
}

void Entity::onUpdate(double dt) {}
void Entity::onDraw(sf::RenderTarget& target) {}
void Entity::onHandleEvent(sf::Event& event) {}
