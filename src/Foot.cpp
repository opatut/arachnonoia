#include "Foot.hpp"

#include <Thor/Math.hpp>

Foot::Foot() {
    m_mass = 0.01f;
    m_physicsShape = new btCompoundShape();

    auto trans_left = btTransform::getIdentity();
    trans_left.setOrigin(btVector3(-0.5, 0, 0));
    m_leftFootShape = new btSphereShape(0.2);

    auto trans_right = btTransform::getIdentity();
    trans_right.setOrigin(btVector3(0.5, 0, 0));
    m_rightFootShape = new btSphereShape(0.2);

    static_cast<btCompoundShape*>(m_physicsShape)->addChildShape(trans_left, m_leftFootShape);
    static_cast<btCompoundShape*>(m_physicsShape)->addChildShape(trans_right, m_rightFootShape);
}

void Foot::onDraw(sf::RenderTarget &target) {
    for(auto i = 0; i < 2; ++i) {
        sf::CircleShape shape;
        auto child = static_cast<btCompoundShape*>(m_physicsShape)->getChildTransform(i);
        auto trans = m_physicsBody->getWorldTransform() * child;
        shape.setPosition(trans.getOrigin().x(), trans.getOrigin().y());
        shape.setRadius(1);
        shape.setOrigin(1, 1);
        shape.setScale(0.2, 0.2);
        shape.setFillColor(sf::Color::Black);
        target.draw(shape);
    }
}

void Foot::onAdd(State* state) {
    m_physicsBody->setDamping(0.5, 5);
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}
