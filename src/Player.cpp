#include "Player.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include <CppTweener.h>

#include "Root.hpp"
#include "Marker.hpp"
#include "Pair.hpp"

Player::Player()
{
    m_sprite.setTexture(* Root().resources.getTexture("player").get());

    m_mass = 1.f;
    m_physicsShape = new btSphereShape(0.3);
    m_zLevel = 1000;
}

std::string Player::getTypeName() {
    return "Player";
}

void Player::onUpdate(double dt) {
    btVector3 ZAXIS(0, 0, 1);
    btVector3 lin = m_physicsBody->getLinearVelocity();
    lin = lin.rotate(ZAXIS, -m_rotation);

    float speed = 1.5;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        lin.setX(speed);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        lin.setX(-speed);
    } else {
        lin.setX(0);
    }
    lin =lin.rotate(ZAXIS, m_rotation);
    m_physicsBody->setLinearVelocity(lin);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_physicsBody->applyCentralForce(btVector3(0, -12, 0));
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_physicsBody->applyCentralForce(btVector3(0, 12, 0));
    }

    m_physicsBody->applyCentralForce(btVector3(0, -9.81, 0));
    btVector3 down(0, -9.81, 0);
    down = down.rotate(btVector3(0, 0, 1), m_rotation);
    m_physicsBody->applyCentralForce(down);

    m_ghostObject->setWorldTransform(m_physicsBody->getWorldTransform());

    // check ghost collisions
    auto m = m_state->getBodyContacts(m_ghostObject);
    btVector3 total(0, 0, 0);
    for(auto pair : m) {
        if(pair.first == this) continue;
        for(auto c : pair.second) {
            if(c.other->getTypeName() == "CollisionShape" || c.other->getTypeName() == "Toy") {
                total += c.position - m_ghostObject->getWorldTransform().getOrigin();
            }
        }
    }

    float targetRotation = thor::Pi;
    if(total.length2() > 0) {
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
}

void Player::onDraw(sf::RenderTarget& target) {
    // Draw body
    sf::CircleShape body;
    body.setPosition(m_position.x, m_position.y);
    body.setRadius(1);
    body.setOrigin(1, 1);
    body.setScale(0.2, m_scale_y);
    body.setFillColor(sf::Color::Black);
    body.setRotation(thor::toDegree(m_rotation));
    target.draw(body);

    // Stuff for legs

    // Offset pos for leg ankle mounts relative to body

    sf::Vector2f offsetAnkles(0, 0.5);
    sf::Vector2f relAnkleMount = thor::rotatedVector(offsetAnkles, thor::toDegree(m_rotation));
    sf::Vector2f absAnkleMount(m_position.x + relAnkleMount.x, m_position.y + relAnkleMount.y);

    // Draw Holy Ray Machine Hover Thing
    sf::RectangleShape hoverThing(sf::Vector2f(1, 0.1));
    hoverThing.setPosition(absAnkleMount);
    hoverThing.setOrigin(0.5, 0.05);
    hoverThing.setRotation(thor::toDegree(m_rotation));
    hoverThing.setFillColor(sf::Color::Green);
    target.draw(hoverThing);

    // Draw legs

    // Leg 1

    // Leg 1 Ankle
    sf::Vector2f offsetLeg1Ankle(offsetAnkles.x - 0.5, offsetAnkles.y);
    sf::Vector2f relLeg1Ankle = thor::rotatedVector(offsetLeg1Ankle, thor::toDegree(m_rotation));
    sf::Vector2f absLeg1Ankle(m_position.x + relLeg1Ankle.x, m_position.y + relLeg1Ankle.y);

    sf::CircleShape debugLeg1Ankle;
    debugLeg1Ankle.setPosition(absLeg1Ankle);
    debugLeg1Ankle.setRadius(1);
    debugLeg1Ankle.setOrigin(0.5, 0.5);
    debugLeg1Ankle.setScale(0.05, 0.05);
    debugLeg1Ankle.setFillColor(sf::Color::Red);
    target.draw(debugLeg1Ankle);

    // Leg 1 Foot
    sf::Vector2f relLeg1FootRayEnd(offsetLeg1Ankle.x - 0.1, offsetLeg1Ankle.y - 1);
    thor::rotate(relLeg1FootRayEnd, thor::toDegree(m_rotation));
    sf::Vector2f absLeg1FootRayEnd(m_position.x + relLeg1FootRayEnd.x, m_position.y + relLeg1FootRayEnd.y);

    sf::CircleShape debugLeg1FootRayEnd;
    debugLeg1FootRayEnd.setPosition(absLeg1FootRayEnd);
    debugLeg1FootRayEnd.setRadius(1);
    debugLeg1FootRayEnd.setOrigin(0.5, 0.5);
    debugLeg1FootRayEnd.setScale(0.05, 0.05);
    debugLeg1FootRayEnd.setFillColor(sf::Color::Yellow);
    target.draw(debugLeg1FootRayEnd);

    btVector3 rayStart(absLeg1Ankle.x, absLeg1Ankle.y, 0);
    btVector3 rayEnd(absLeg1FootRayEnd.x, absLeg1FootRayEnd.y, 0);
    btDiscreteDynamicsWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);

    m_state->dynamicsWorld()->rayTest(rayStart, rayEnd, rayCallback);

    if(rayCallback.hasHit()) {
        btVector3 hitPoint = rayCallback.m_hitPointWorld;

        sf::CircleShape debugLeg1FootRayHit;
        debugLeg1FootRayHit.setPosition(hitPoint.x(), hitPoint.y());
        debugLeg1FootRayHit.setRadius(1);
        debugLeg1FootRayHit.setOrigin(0.5, 0.5);
        debugLeg1FootRayHit.setScale(0.05, 0.05);
        debugLeg1FootRayHit.setFillColor(sf::Color::Blue);
        target.draw(debugLeg1FootRayHit);
    }

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
            Root().game_state.message("Goal reached");
        }
    }
    return false;
}

int Player::getPairsLeft() const {
    auto e = m_state->getEntitiesByType<Pair>("Pair");
    return e.size() - std::count_if(e.begin(), e.end(), [](std::shared_ptr<Pair> p) -> bool { return p->isSolved(); });
}
