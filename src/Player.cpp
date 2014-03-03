#include "Player.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include <CppTweener.h>

#include "Root.hpp"
#include "Pair.hpp"
#include "Foot.hpp"

Player::Player()
{
    m_sprite.setTexture(* Root().resources.getTexture("player").get());

    m_mass = 1.f;
    m_physicsShape = new btSphereShape(0.3);

    // Create foreground feet
    for(size_t i = 0; i < 4; ++i) {
        m_foregroundFeet.push_back(std::make_shared<Foot>(this, i));
    }
}

std::string Player::getTypeName() const {
    return "Player";
}

void Player::onUpdate(double dt) {
    auto lin = m_physicsBody->getLinearVelocity();
    float speed = 1.5;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        // Moving backward
        m_physicsBody->setLinearVelocity(btVector3(-speed, lin.y(), lin.z()));
        for(auto foot : m_foregroundFeet)
            foot->setDirection(-1);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        // Moving forward
        m_physicsBody->setLinearVelocity(btVector3(speed, lin.y(), lin.z()));
        for(auto foot : m_foregroundFeet)
            foot->setDirection(1);
    } else {
        // Falling but not actively moving
        m_physicsBody->setLinearVelocity(btVector3(0, lin.y(), 0));
        for(auto foot : m_foregroundFeet)
            foot->setDirection(0);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        // TODO
        // This probably shouldn't exist
        m_physicsBody->applyCentralForce(btVector3(0, -12, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        // TODO
        // Preparing to jump
        m_physicsBody->applyCentralForce(btVector3(0, 12, 0));
    }

    for(auto foot : m_foregroundFeet)
        foot->onUpdate(dt);

    // Apply manual gravity in the direction of current rotation to simulate stickyness to walls
    m_physicsBody->applyCentralForce(btVector3(0, -9.81, 0));
    btVector3 down(0, -9.81, 0);
    down = down.rotate(btVector3(0, 0, 1), m_rotation);
    m_physicsBody->applyCentralForce(down);

    m_ghostObject->setWorldTransform(m_physicsBody->getWorldTransform());

    // Check ghost collisions
    auto m = m_state->getBodyContacts(m_ghostObject);
    btVector3 total(0, 0, 0);
    for(auto pair : m) {
        if(pair.first == this) continue;
        for(auto c : pair.second) {
            total += c.position - m_ghostObject->getWorldTransform().getOrigin();
        }
    }
    if(total.length2() > 0) {
        setPhysicsRotation(thor::Pi / 2 + atan2(total.y(), total.x()));
    } else {
        setPhysicsRotation(thor::Pi);
    }
}

void Player::onDraw(State* state, sf::RenderTarget& target) {
    // Draw body
    sf::CircleShape body;
    body.setPosition(m_position.x, m_position.y);
    body.setRadius(1);
    body.setOrigin(1, 1);
    body.setScale(0.2, m_scale_y);
    body.setFillColor(sf::Color::Black);
    body.setRotation(thor::toDegree(m_rotation));
    target.draw(body);

    // Draw foreground legs
    for(auto foot : m_foregroundFeet)
        foot->onDraw(target);

    // Draw eyes
    for(auto i = 0; i < 2; ++i) {
        sf::CircleShape eye;
        sf::Vector2f pos(m_position.x + (0.5-i)*0.1, m_position.y + 0.05);
        sf::Vector2f diff(m_position.x - pos.x, m_position.y - pos.y);
        thor::rotate(diff, thor::toDegree(m_rotation));
        eye.setPosition(m_position.x + diff.x, m_position.y + diff.y);
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
    m_physicsBody->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK|btCollisionObject::CF_CHARACTER_OBJECT);
    m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);

    // set up ghost object
    m_ghostObject = new btGhostObject();
    m_ghostObject->setCollisionShape(new btSphereShape(0.35));
    m_ghostObject->setCollisionFlags(m_ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    // m_ghostObject->setUserPointer((void*)this);
    m_state->dynamicsWorld()->addCollisionObject(m_ghostObject, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);

    m_scale_y = 0.18;
    tween::TweenerParam param(1000, tween::SINE, tween::EASE_IN_OUT);
    param.addProperty(&m_scale_y, .15f);
    param.setRepeatWithReverse(10000, true);
    state->m_tweener.addTween(param);
}

bool Player::onCollide(Entity* other, const EntityCollision& c) {
    if(other->getTypeName() == "Pair") {
        ((Pair*)other)->activate();
        return true;
    }
    return false;
}

int Player::getPairsLeft() const {
    auto e = m_state->getEntitiesByType<Pair>("Pair");
    return e.size() - std::count_if(e.begin(), e.end(), [](std::shared_ptr<Pair> p) -> bool { return p->isSolved(); });
}
