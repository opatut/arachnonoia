#include "Player.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include "Root.hpp"

Player::Player()
{
    m_sprite.setTexture(* Root().resources.getTexture("player").get());

    m_mass = 1.f;
    m_physicsShape = new btSphereShape(0.3);
}

void Player::onUpdate(double dt) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_physicsBody->applyCentralForce(btVector3(-5, 0, 0));
        for(auto i = 0; i < 2; ++i) {
            m_feet[i]->physicsBody()->applyTorque(btVector3(0, 0, -0.5f));
        }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_physicsBody->applyCentralForce(btVector3(5, 0, 0));
        for(auto i = 0; i < 2; ++i) {
            m_feet[i]->physicsBody()->applyTorque(btVector3(0, 0, 0.5f));
        }
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_physicsBody->applyCentralForce(btVector3(0, -5, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_physicsBody->applyCentralForce(btVector3(0, 5, 0));
    }
}

void Player::onDraw(sf::RenderTarget& target) {
//    m_sprite.setPosition(m_position.x, m_position.y);
//    m_sprite.setScale(0.005, 0.005);
//    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
//    m_sprite.setRotation(thor::toDegree(m_rotation));
//    m_sprite.setColor(sf::Color(255, 255, 255, 100));
//    target.draw(m_sprite);

    // Draw body
    sf::CircleShape body;
    body.setPosition(m_position.x, m_position.y);
    body.setRadius(1);
    body.setOrigin(1, 1);
    body.setScale(0.2, 0.2);
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

    // Set up spider feet
    for(auto i = 0; i < 4; ++i) {
        auto foot = std::make_shared<Foot>(this);

        if(i == 0) // left feet
            foot->setPosition(glm::vec2(-1, 0.3));
        if(i == 1) // right feet
            foot->setPosition(glm::vec2(1, 0.3));
        if(i == 2) // right feet
            foot->setPosition(glm::vec2(-2, 0.3));
        if(i == 3) // right feet
            foot->setPosition(glm::vec2(2, 0.3));

        state->add(foot);
        m_feet[i] = foot;

        auto frameInA = btTransform::getIdentity();
        frameInA.setOrigin(foot->physicsBody()->getWorldTransform().getOrigin());

        auto frameInB = btTransform::getIdentity();
        if(i == 0) // left feet
            frameInB.setOrigin(foot->physicsBody()->getWorldTransform().getOrigin() + btVector3(1, -0.3, 0));
        if(i == 1) // right feet
            frameInB.setOrigin(foot->physicsBody()->getWorldTransform().getOrigin() + btVector3(-1, -0.3, 0));

        auto constraint = new btGeneric6DofSpringConstraint(*m_physicsBody, *foot->physicsBody(), frameInA, frameInB, true);
        constraint->setLinearLowerLimit(btVector3(0.0, 0.4, 0));
        constraint->setLinearUpperLimit(btVector3(0.0, 0.5, 0));
        constraint->setAngularLowerLimit(btVector3(0.0, 0.0, -M_PI));
        constraint->setAngularUpperLimit(btVector3(0.0, 0.0, M_PI));

        constraint->enableSpring(1, true);
        constraint->setStiffness(1, 500.f);
        constraint->setDamping(1, 0.5f);

        state->dynamicsWorld()->addConstraint(constraint, true);
    }

    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}
