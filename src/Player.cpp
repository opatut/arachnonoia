#include "Player.hpp"

#include <iostream>

#include "Root.hpp"

Player::Player()
{
    m_sprite.setTexture(* Root().resources.getTexture("player").get());

    m_mass = 1.f;
    m_physicsShape = new btSphereShape(0.5);
}

void Player::onUpdate(double dt) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_physicsBody->applyCentralForce(btVector3(-5, 0, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_physicsBody->applyCentralForce(btVector3(5, 0, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_physicsBody->applyCentralForce(btVector3(0, -5, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_physicsBody->applyCentralForce(btVector3(0, 5, 0));
    }

    std::cout << "player: " << m_physicsBody->getWorldTransform().getOrigin().x() << "/" << m_physicsBody->getWorldTransform().getOrigin().y() << std::endl;
}

void Player::onDraw(sf::RenderTarget& target) {
    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(0.005, 0.005);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    m_sprite.setColor(sf::Color(255, 255, 255, 100));
    target.draw(m_sprite);

    sf::CircleShape body;
    body.setPosition(m_position.x, m_position.y);
    body.setRadius(1);
    body.setOrigin(1, 1);
    body.setScale(0.1, 0.05);
    body.setFillColor(sf::Color::Black);
    target.draw(body);

    // right leg
    // glm::vec2 foot(0.5, 0.2);
    // glm::vec2 root(0, 0);
    // float l1 = 0.2, l2 = 0.5;
}

void Player::onAdd() {
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}
