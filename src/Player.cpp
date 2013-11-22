#include "Player.hpp"

#include <iostream>
#include "Root.hpp"

Player::Player()
    : Entity()
{
    position = glm::vec2(0, 0);
    m_sprite.setTexture(* Root::instance().resources.getTexture("player").get());
}

void Player::onUpdate(double dt) {
    float speed = 0.5;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        position.x -= dt * speed;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        position.x += dt * speed;
    }
}

void Player::onDraw(sf::RenderTarget& target) {
    m_sprite.setPosition(position.x, position.y);
    m_sprite.setScale(0.005, 0.005);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    target.draw(m_sprite);
}
