#include "Player.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include <CppTweener.h>

#include "Root.hpp"
#include "Marker.hpp"
#include "Pair.hpp"
#include "Foot.hpp"

Player::Player() {
    m_sprite.setTexture(* Root().resources.getTexture("player").get());

    m_mass = 1.f;
    m_ability = WALK;
    m_physicsShape = new btSphereShape(0.3);

    // Create foreground feet
    for(size_t i = 0; i < 4; ++i) {
        m_foregroundFeet.push_back(std::make_shared<Foot>(this, i));
    }

    m_zLevel = 1000;
    m_rotation = thor::Pi;
}

std::string Player::getTypeName() const {
    return "Player";
}

void Player::onUpdate(double dt) {
    // Check ghost collisions
    m_ghostObject->setWorldTransform(m_physicsBody->getWorldTransform());
    auto m = m_state->getBodyContacts(m_ghostObject);
    btVector3 total(0, 0, 0);
    for(auto pair : m) {
        if(pair.first == this) continue;
        for(auto c : pair.second) {
            if(c.other->getTypeName() == "CollisionShape" || c.other->getTypeName() == "Toy") {
                auto d = c.position - m_ghostObject->getWorldTransform().getOrigin();
                if(d.y() > 0 || m_ability >= WALLS) {
                    total += d;
                }
            }
        }
    }

    bool onGround = total.length2() > 0;

    // set rotation
    float targetRotation = thor::Pi;
    if(onGround) {
        targetRotation = thor::Pi / 2 + atan2(total.y(), total.x());
    }
    if(m_rotation - targetRotation > thor::Pi) {
        targetRotation += thor::Pi * 2;
    }
    if(m_rotation - targetRotation < -thor::Pi) {
        targetRotation -= thor::Pi * 2;
    }

    float rotSpeed = 10;
    float rot = m_rotation * (1 - rotSpeed * dt) + targetRotation * (rotSpeed * dt);
    setPhysicsRotation(rot);

    // movement
    btVector3 ZAXIS(0, 0, 1);
    btVector3 lin = m_physicsBody->getLinearVelocity();
    lin = lin.rotate(ZAXIS, -m_rotation);

    if(m_ability >= WALK) {
        float walkSpeed = 1.5;
        float airAccel = 2.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if(onGround) {
                lin.setX(walkSpeed);
            } else {
                lin.setX(lin.getX() + airAccel * dt);
            }
            for(auto foot : m_foregroundFeet) foot->setDirection(-1);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if(onGround) {
                lin.setX(-walkSpeed);
            } else {
                lin.setX(lin.getX() - airAccel * dt);
            }
            for(auto foot : m_foregroundFeet) foot->setDirection(1);
        } else {
            if(onGround) {
                lin.setX(0);
            }
            for(auto foot : m_foregroundFeet) foot->setDirection(0);
        }
        lin = lin.rotate(ZAXIS, m_rotation);
        m_physicsBody->setLinearVelocity(lin);
    }

    if(m_ability >= JUMP) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            float powerSpeed = 2.f;
            m_springPower = fmin(1, m_springPower + powerSpeed * dt);
        }
    }

    // update feed
    for(auto foot : m_foregroundFeet) foot->onUpdate(dt);

    // Apply manual gravity in the direction of current rotation to simulate stickyness to walls
    if(m_ability >= WALLS) {
        m_physicsBody->applyCentralForce(btVector3(0, -9.81, 0));
        m_physicsBody->applyCentralForce(btVector3(0, -5, 0).rotate(btVector3(0, 0, 1), m_rotation));
    }

}

void Player::onDraw(sf::RenderTarget& target) {
    // Draw body
    sf::CircleShape body;
    body.setPosition(m_position.x, m_position.y);
    body.setRadius(1);
    body.setOrigin(1, 1 + m_springPower);
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
        Pair* p = (Pair*)other;
        p->activate();
        return true;
    } else if(other->getTypeName() == "Marker") {
        Marker* m = (Marker*)other;
        if(m->getType() == Marker::GOAL) {
            auto p = getPairsLeft();
            if(p == 0) {
                Root().game_state.nextLevel();
            } else {
                Root().game_state.message(std::to_string(p / 2) + " pair" + (p == 2 ? "" : "s") + " missing");
            }
        }
    }
    return false;
}

void Player::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::Space) {
            jump();
        }
    }
}

void Player::jump() {
    if(m_ability < JUMP) return;

    float strength = 7.f * m_springPower;
    btVector3 impulse(0, -1, 0);
    impulse = impulse.rotate(btVector3(0, 0, 1), thor::Pi + m_rotation);
    impulse *= strength;
    m_physicsBody->applyCentralImpulse(impulse);

    m_springPower = 0;
}

int Player::getPairsLeft() const {
    auto e = m_state->getEntitiesByType<Pair>("Pair");
    return e.size() - std::count_if(e.begin(), e.end(), [](std::shared_ptr<Pair> p) -> bool { return p->isSolved(); });
}

void Player::setAbility(Player::Ability ability) {
    m_ability = ability;
}

Player::Ability Player::getAbility() const {
    return m_ability;
}
