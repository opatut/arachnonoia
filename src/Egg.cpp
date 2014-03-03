#include "Egg.hpp"

#include "Root.hpp"
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

Egg::Egg(Type type)
    : m_progress(0),
      m_type(type) {
    m_mass = 0.3f;
}

std::string Egg::getTypeName() const {
    return "Egg";
}

void constructHalfSphere(btConvexHullShape* shape, float angle) {
    btVector3 s(0.66, 1.0, 1.0);
    for(int i = 0; i <= 12; ++i) {
        shape->addPoint(btVector3(0.5, 0, 0).rotate(btVector3(0, 0, 1), angle + thor::Pi * ((i+0)%13) / 12.f) * s);
        shape->addPoint(btVector3(0.5, 0, 0).rotate(btVector3(0, 0, 1), angle + thor::Pi * ((i+1)%13) / 12.f) * s);
        shape->addPoint(btVector3(0.5, 0, 1).rotate(btVector3(0, 0, 1), angle + thor::Pi * ((i+1)%13) / 12.f) * s);
    }
}

void Egg::onInitialize() {
    auto shape = new btConvexHullShape();
    if(m_type != UPPER) constructHalfSphere(shape, 0);
    if(m_type != LOWER) constructHalfSphere(shape, thor::Pi);

    m_physicsShape = shape;

    // if(m_type == FULL) {
    //     m_physicsShape = new btSphereShape(0.5);
    //     m_physicsShape->setLocalScaling(btVector3(0.6, 1.f, 1.f));
    // } else {
    //     m_physicsShape = new btSphereShape(0.2);
    // }
}

void Egg::onUpdate(double dt) {
    if(m_type == FULL) {
        if(m_lifeTime < 2.f) {
            m_progress = 0.0;
        } else if(m_lifeTime < 3.f) {
            m_progress = 0.3;
        } else if(m_lifeTime < 3.8f) {
            m_progress = 0.4;
        } else if(m_lifeTime < 4.f) {
            m_progress = 0.7;
        } else {
            if(m_progress < 1) {
                Root().game_state.spawnPlayer(m_position + glm::vec2(0, -0.4));

                auto upper = std::make_shared<Egg>(UPPER);
                upper->setPhysicsPosition(m_position + glm::vec2(0, -0.7));
                m_state->add(upper);
                upper->physicsBody()->applyCentralImpulse(btVector3(-1, -1, 0) * 0.8);
                upper->physicsBody()->applyTorque(btVector3(0, 0, -5));

                auto lower = std::make_shared<Egg>(LOWER);
                lower->setPhysicsPosition(m_position + glm::vec2(0, -0.1));
                m_state->add(lower);

                kill();
            }
            m_progress = 1.0;
        }
    } else {
        m_progress = 1.f;
    }
}

void Egg::onDraw(sf::RenderTarget& target) {
    if(m_type == FULL) {
        if(m_progress < 1) {
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
            crack.setScale(1.f / tex->getSize().y * m_scale.x * m_progress, 1.f / tex->getSize().y * m_scale.y);
            crack.setRotation(thor::toDegree(m_rotation));
            target.draw(crack);
        }
    } else {
        auto tex = Root().resources.getTexture(m_type == UPPER ? "egg-top" : "egg-bottom");
        sf::Sprite egg(*tex.get()); 
        egg.setPosition(m_position.x, m_position.y);
        egg.setOrigin(tex->getSize().x / 2, tex->getSize().y / 2);
        egg.setScale(1.f / tex->getSize().y * m_scale.x, 1.f / tex->getSize().y * m_scale.y);
        egg.setRotation(thor::toDegree(m_rotation));
        target.draw(egg);
    }
}

void Egg::onHandleEvent(sf::Event& event) {

}
