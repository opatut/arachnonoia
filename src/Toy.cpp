#include "Toy.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include <CppTweener.h>

#include "Root.hpp"
#include "Pair.hpp"

Toy::Toy() {
    m_mass = 1.f;
    m_zLevel = 800;
    m_physicsShape = new btBoxShape(btVector3(0.5, 0.5, 1));
}

std::string Toy::getTypeName() {
    return "Toy";
}

void Toy::onUpdate(double dt) {
    m_physicsShape->setLocalScaling(btVector3(m_scale.x, m_scale.y, 1));
}

void Toy::onDraw(sf::RenderTarget& target) {
    auto tex = Root().resources.getTexture("wall-box");
    glm::vec2 s(tex->getSize().x, tex->getSize().y);

    sf::Sprite sprite(*tex.get());
    sprite.setPosition(m_position.x, m_position.y);
    sprite.setRotation(thor::toDegree(m_rotation));
    sprite.setScale(m_scale.x / s.x, m_scale.y / s.y);
    sprite.setOrigin(s.x / 2, s.y / 2);
    target.draw(sprite);
}

void Toy::onAdd(State* state) {
    m_physicsBody->setDamping(0.5, 5);
    m_physicsBody->setAngularFactor(0.2);
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}
