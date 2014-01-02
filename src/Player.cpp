#include "Player.hpp"

#include <iostream>

#include "Root.hpp"

Player::Player()
{
    m_position = glm::vec2(0, 0);
    m_sprite.setTexture(* Root::instance().resources.getTexture("player").get());
}

void Player::onUpdate(double dt) {
    float speed = 0.5;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_position.x -= dt * speed;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_position.x += dt * speed;
    }
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
    glm::vec2 foot(0.5, 0.2);
    glm::vec2 root(0, 0);
    float l1 = 0.2, l2 = 0.5;
}
