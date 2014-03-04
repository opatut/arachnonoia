#include "Egg.hpp"

#include "Root.hpp"
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtx/vector_angle.hpp>

Egg::Egg(Type type)
    : m_progress(0),
      m_type(type) {
    m_mass = 0.5f;
    m_rotation = thor::Pi / 2;
    m_scale = glm::vec2(0.8, 0.8);
    m_hatching = false;
}

std::string Egg::getTypeName() const {
    return "Egg";
}

void constructHalfSphere(btConvexHullShape* shape, float angle, btVector3 offset) {
    btVector3 s(0.55, 0.85, 1.0);
    for(int i = 0; i <= 12; ++i) {
        shape->addPoint(offset + btVector3(0.5, 0, 0).rotate(btVector3(0, 0, 1), angle + thor::Pi * ((i+0)%13) / 12.f) * s);
        shape->addPoint(offset + btVector3(0.5, 0, 0).rotate(btVector3(0, 0, 1), angle + thor::Pi * ((i+1)%13) / 12.f) * s);
        shape->addPoint(offset + btVector3(0.5, 0, 1).rotate(btVector3(0, 0, 1), angle + thor::Pi * ((i+1)%13) / 12.f) * s);
    }
}

void Egg::onInitialize() {
    auto shape = new btConvexHullShape();
    if(m_type == FULL) {
        constructHalfSphere(shape, 0,        btVector3(0, 0, 0));
        constructHalfSphere(shape, thor::Pi, btVector3(0, 0, 0));
    } else if(m_type == LOWER) {
        constructHalfSphere(shape, 0,        btVector3(0, -0.2, 0));
    } else if(m_type == UPPER) {
        constructHalfSphere(shape, thor::Pi, btVector3(0, 0.2, 0));
    }

    m_physicsShape = shape;

    // if(m_type == FULL) {
    //     m_physicsShape = new btSphereShape(0.5);
    //     m_physicsShape->setLocalScaling(btVector3(0.6, 1.f, 1.f));
    // } else {
    //     m_physicsShape = new btSphereShape(0.2);
    // }
}

void Egg::onAdd(State* state) {
    m_physicsShape->setLocalScaling(btVector3(m_scale.x, m_scale.y, 1));
    m_physicsBody->setFriction(2.5);
    if(!m_hatching) {
        m_physicsBody->setDamping(0.5, 0.7f);
    } else {
        m_physicsBody->forceActivationState(DISABLE_DEACTIVATION);
    }
}

void Egg::onUpdate(double dt) {
    m_physicsShape->setLocalScaling(btVector3(m_scale.x, m_scale.y, 1));

    if(m_type == FULL && m_hatching) {
        if(m_lifeTime < 2.f) {
            m_progress = 0.0;
        } else if(m_lifeTime < 4.5f) {
            m_progress = (m_lifeTime - 2.f) / 1.8;
            m_progress = pow(m_progress, 10);
            m_progress = fmin(0.95, ceil(m_progress * 5) / 5);
        } else {
            if(m_progress < 1) {
                kill();
                Root().game_state.spawnPlayer(m_position + glm::vec2(0, -0.4));

                btVector3 ZAXIS(0, 0, 1);

                auto upper = std::make_shared<Egg>(UPPER);
                upper->setPhysicsPosition(m_position + glm::rotate(glm::vec2(0, -0.25 * m_scale.y), m_rotation));
                upper->setPhysicsRotation(m_rotation);
                m_state->add(upper);
                // upper->physicsBody()->applyCentralImpulse(btVector3(0, -1, 0).rotate(ZAXIS, m_rotation) * 0.1);
                upper->physicsBody()->applyTorque(btVector3(0, 0,  1));

                auto lower = std::make_shared<Egg>(LOWER);
                lower->setPhysicsPosition(m_position + glm::rotate(glm::vec2(0, 0.25 * m_scale.y), m_rotation));
                lower->setPhysicsRotation(m_rotation);
                m_state->add(lower);
                // lower->physicsBody()->applyCentralImpulse(btVector3(0, 1, 0).rotate(ZAXIS, m_rotation) * 0.1);
                lower->physicsBody()->applyTorque(btVector3(0, 0, -1));
            }
            m_progress = 1.0;
        }
    } else {
        m_progress = 1.f;
    }
}

void Egg::onDraw(sf::RenderTarget& target) {
    if(m_type == FULL) {
        if(m_progress < 1 || !m_hatching) {
            auto tex = Root().resources.getTexture("egg");
            sf::Sprite egg(*tex.get()); 
            egg.setPosition(m_position.x, m_position.y);
            egg.setOrigin(tex->getSize().x / 2, tex->getSize().y / 2);
            egg.setScale(1.f / tex->getSize().y * m_scale.x, 1.f / tex->getSize().y * m_scale.y);
            egg.setRotation(thor::toDegree(m_rotation));
            target.draw(egg);
        }

        if(m_progress > 0 && m_progress < 1) {
            auto tex = Root().resources.getTexture("egg-crack");
            sf::Sprite crack(*tex.get()); 
            crack.setPosition(m_position.x, m_position.y);
            crack.setOrigin(tex->getSize().x / 2, tex->getSize().y / 2);
            crack.setScale(1.f / tex->getSize().y * m_scale.x, 1.f / tex->getSize().y * m_scale.y);
            crack.setTextureRect(sf::IntRect(0, 0, tex->getSize().x * m_progress, tex->getSize().y));
            crack.setRotation(thor::toDegree(m_rotation));
            target.draw(crack);
            target.draw(crack);
        }
    } else {
        auto tex = Root().resources.getTexture(m_type == UPPER ? "egg-top" : "egg-bottom");
        sf::Sprite egg(*tex.get()); 
        egg.setPosition(m_position.x, m_position.y);
        egg.setOrigin(tex->getSize().x / 2, tex->getSize().y * (0.5 + (m_type == UPPER ? -0.2 : 0.2)));
        egg.setScale(1.f / tex->getSize().y * m_scale.x, 1.f / tex->getSize().y * m_scale.y);
        egg.setRotation(thor::toDegree(m_rotation));
        target.draw(egg);
    }
}

void Egg::onHandleEvent(sf::Event& event) {

}

void Egg::setHatching(bool hatching) {
    m_hatching = true;
}
