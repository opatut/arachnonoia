#include "Wall.hpp"

#include <Thor/Math.hpp>

#include "Root.hpp"

const std::string Wall::types[] = {
    "box",
    "blah"
};

Wall::Wall() {
    m_type = types[0];

    m_mass = 0;
    m_scale = glm::vec2(1/256.f, 1/256.f);
    m_sprite.setTexture(* Root().resources.getTexture("wall-" + m_type).get());

    // maybe split this physics stuff out into its own function if the logic becomes complex
    m_physicsShape = new btBoxShape(btVector3(0.5f, 0.5f, 0));
}

void Wall::onUpdate(double dt) {}

void Wall::onDraw(sf::RenderTarget& target) {
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(m_scale.x, m_scale.y);
    m_sprite.setRotation(thor::toDegree(m_rotation));
    target.draw(m_sprite);
}

glm::vec2 Wall::getSize() {
    return glm::vec2(m_sprite.getTexture()->getSize().x, m_sprite.getTexture()->getSize().y);
}
