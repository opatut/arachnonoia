#include "Foot.hpp"

Foot::Foot() {
    m_mass = 0.5f;
    m_physicsShape = new btSphereShape(0.15);
}

void Foot::onDraw(sf::RenderTarget &target) {
    sf::CircleShape foot;
    foot.setPosition(m_position.x, m_position.y);
    foot.setRadius(1);
    foot.setOrigin(1, 1);
    foot.setScale(0.15, 0.15);
    foot.setFillColor(sf::Color::Black);
    target.draw(foot);
}

void Foot::onAdd(State *state) {
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}
