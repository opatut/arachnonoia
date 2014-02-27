#include "Player.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include <CppTweener.h>

#include "Root.hpp"
#include "Pair.hpp"

Player::Player()
{
    m_sprite.setTexture(* Root().resources.getTexture("player").get());

    m_mass = 1.f;
    m_physicsShape = new btSphereShape(0.3);
}

std::string Player::getTypeName() {
    return "Player";
}

void Player::onUpdate(double dt) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_physicsBody->applyCentralForce(btVector3(-5, 0, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_physicsBody->applyCentralForce(btVector3(5, 0, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_physicsBody->applyCentralForce(btVector3(0, -12, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_physicsBody->applyCentralForce(btVector3(0, 5, 0));
    }
}

void Player::onDraw(sf::RenderTarget& target) {
    // Draw body
    sf::CircleShape body;
    body.setPosition(m_position.x, m_position.y);
    body.setRadius(1);
    body.setOrigin(1, 1);
    body.setScale(0.2, m_scale_y);
    body.setFillColor(sf::Color::Black);
    target.draw(body);

    // Draw eyes
    for(auto i = 0; i < 2; ++i) {
        sf::CircleShape eye;
        eye.setPosition(m_position.x + (0.5-i)*0.1, m_position.y + 0.05);
        eye.setRadius(1);
        eye.setOrigin(1, 1);
        eye.setScale(0.01, 0.01);
        eye.setFillColor(sf::Color::White);
        target.draw(eye);
    }
}

void Player::onAdd(State* state) {
    m_physicsBody->setDamping(0.5, 5);
    m_physicsBody->setAngularFactor(0.2);
    m_physicsBody->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);

    m_scale_y = 0.2;
    tween::TweenerParam param(1000, tween::SINE, tween::EASE_IN_OUT);
    param.addProperty(&m_scale_y, .15f);
    param.setRepeatWithReverse(10000, true);
    state->m_tweener.addTween(param);

}

bool Player::onCollide(Entity* other) {
    if(other->getTypeName() == "Pair") {
        ((Pair*)other)->activate();
        return true;
    }
}
