#include "Wall.hpp"

#include <Thor/Math.hpp>

#include "Root.hpp"

const std::string Wall::types[] = {
    "box",
    "platform-1",
    "platform-2"
};

Wall::Wall() {
    setType(types[0]);
    m_mass = 0;

    // maybe split this physics stuff out into its own function if the logic becomes complex
    m_physicsShape = new btBoxShape(btVector3(0.5f, 0.5f, 1));
}

std::string Wall::getTypeName() {
    return "Wall";
}

void Wall::onUpdate(double dt) {
    m_physicsShape->setLocalScaling(btVector3(m_scale.x, m_scale.y, 1));
}

void Wall::onDraw(State* state, sf::RenderTarget& target) {
    auto s = getSize();
    m_sprite.setOrigin(s.x / 2, s.y / 2);
    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(m_scale.x / s.x, m_scale.y / s.y);
    m_sprite.setRotation(thor::toDegree(m_rotation));
    target.draw(m_sprite);
}

void Wall::setMetadata(int data) {
    data--;
    if(data >= 0 && data < 3) {
        setType(types[data]);
        auto s = getSize();
        m_scale = s / (float)fmax(s.x, s.y);
    }
}

void Wall::onAdd(State* state) {
    setType(m_type);
}

void Wall::setType(const std::string& type) {
    m_type = type;
    m_sprite = sf::Sprite(*Root().resources.getTexture("wall-" + m_type).get());
}

glm::vec2 Wall::getSize() {
    return glm::vec2(m_sprite.getTexture()->getSize().x, m_sprite.getTexture()->getSize().y);
}
