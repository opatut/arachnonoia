#include "Wall.hpp"

#include <iostream>

#include "Root.hpp"

const std::string Wall::types[] = {
    "box",
    "blah"
};

Wall::Wall() {
    m_type = types[0];

    m_position = glm::vec2(0, 0);
    m_sprite.setTexture(* Root().resources.getTexture("wall-" + m_type).get());

    // maybe split this physics stuff out into its own function if the logic becomes complex
    m_physicsShape = new btBoxShape(btVector3(1, 1, 0));
}

void Wall::onUpdate(double dt) {
    btTransform trans;
    m_physicsBody->getMotionState()->getWorldTransform(trans);
    std::cout << trans.getOrigin().getY() << std::endl;
}

void Wall::onDraw(sf::RenderTarget& target) {
    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(0.005, 0.005);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    target.draw(m_sprite);
}
