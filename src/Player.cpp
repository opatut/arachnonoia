#include "Player.hpp"

#include <iostream>
#include "Root.hpp"

Player::Player()
    : Entity()
{
    position = glm::vec2(200, 200);
    m_sprite.setTexture(* Root::instance().resources.getTexture("player").get());
}

void Player::onUpdate(double dt) {
    float speed = 100;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        position.x -= dt * speed;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        position.x += dt * speed;
    }
}

void Player::onDraw(sf::RenderTarget& target) {
    m_sprite.setPosition(position.x, position.y);
    target.draw(m_sprite);
}
