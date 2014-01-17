#include "Foot.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>
#include <Thor/Graphics.hpp>

Foot::Foot(Entity* player) {
    m_mass = 0.01f;
    m_player = player;
    m_physicsShape = new btCompoundShape();

    auto trans_left = btTransform::getIdentity();
    trans_left.setOrigin(btVector3(-0.5, 0, 0));
    m_leftFootShape = new btSphereShape(0.2);

    auto trans_right = btTransform::getIdentity();
    trans_right.setOrigin(btVector3(0.5, 0, 0));
    m_rightFootShape = new btSphereShape(0.2);

    static_cast<btCompoundShape*>(m_physicsShape)->addChildShape(trans_left, m_leftFootShape);
    static_cast<btCompoundShape*>(m_physicsShape)->addChildShape(trans_right, m_rightFootShape);
}

void Foot::onDraw(sf::RenderTarget &target) {
    for(auto i = 0; i < 2; ++i) {
        // Feet
        auto child = static_cast<btCompoundShape*>(m_physicsShape)->getChildTransform(i);
        auto trans = m_physicsBody->getWorldTransform() * child;
        sf::Vector2f feet_pos(trans.getOrigin().x(), trans.getOrigin().y());
        sf::Vector2f player_pos(m_player->position().x, m_player->position().y);

        sf::CircleShape foot;
        foot.setPosition(feet_pos);
        foot.setRadius(1);
        foot.setOrigin(1, 1);
        foot.setScale(0.2, 0.2);
        foot.setFillColor(sf::Color::Black);
        target.draw(foot);

        // Kneecaps
        sf::CircleShape kneecap;
        sf::Vector2f kneecap_pos(feet_pos.x + (0.5-i)*0.5, feet_pos.y - 2);
        kneecap.setPosition(kneecap_pos);
        kneecap.setRadius(1);
        kneecap.setOrigin(1, 1);
        kneecap.setScale(0.1, 0.1);
        kneecap.setFillColor(sf::Color::Black);
        target.draw(kneecap);

        // Lower legs
        sf::RectangleShape lowerleg;
        sf::Vector2f lower_to_kneecap(thor::unitVector(feet_pos - kneecap_pos));
        lowerleg.setPosition(feet_pos);
        lowerleg.setSize(sf::Vector2f(2, 0.1));
        lowerleg.setOrigin(2, 0.05);
        lowerleg.setRotation(thor::polarAngle(lower_to_kneecap));
        lowerleg.setFillColor(sf::Color::Black);
        target.draw(lowerleg);

        // Upper legs
        sf::RectangleShape upperleg;
        sf::Vector2f upper_to_kneecap(thor::unitVector(kneecap_pos - player_pos));
        auto upperleg_length = thor::length(kneecap_pos - player_pos);
        upperleg.setPosition(kneecap_pos);
        upperleg.setSize(sf::Vector2f(upperleg_length, 0.1));
        upperleg.setOrigin(upperleg_length, 0.05);
        upperleg.setRotation(thor::polarAngle(upper_to_kneecap));
        upperleg.setFillColor(sf::Color::Black);
        target.draw(upperleg);
    }
}

void Foot::onAdd(State* state) {
    m_physicsBody->setDamping(0.5, 5);
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}
