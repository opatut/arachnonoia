#include "State.hpp"

#include "Root.hpp"
#include "EntityMotionState.hpp"

#include <iostream>

void State::init() {
    if(m_usePhysics) {
        m_broadphase = new btDbvtBroadphase();
        m_collisionConfiguration = new btDefaultCollisionConfiguration();
        m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
        m_solver = new btSequentialImpulseConstraintSolver;
        m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfiguration);

        m_dynamicsWorld->setGravity(btVector3(0, 1, 0));
    }

    onInit();
}

State::~State() {
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_collisionDispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

void State::update(double dt) {
    onUpdate(dt);
    for(auto entity : m_entities) {
        entity->onUpdate(dt);
    }

    if(m_usePhysics)
        m_dynamicsWorld->stepSimulation(dt, 10);
}

void State::draw(sf::RenderTarget& target) {
    onDraw(target);
}

void State::handleEvent(sf::Event& event) {
    onHandleEvent(event);
    for(auto entity : m_entities) {
        entity->onHandleEvent(event);
    }
}

void State::onInit() {}

void State::onUpdate(double dt) {}

void State::onDraw(sf::RenderTarget& target) {
    drawEntities(target);
}

void State::onHandleEvent(sf::Event& event) {}

void State::add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);

    // if there is no physics shape set, the entity probably doesn't like physics so leave it alone
    if(entity->physicsShape() != nullptr && m_usePhysics) {
        EntityMotionState* motionstate = new EntityMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)), entity);
        entity->setMotionState(motionstate);
        btScalar mass = 1;
        btVector3 inertia(0, 0, 0);
        entity->physicsShape()->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo construction_info(mass, motionstate, entity->physicsShape(), inertia);
        entity->setPhysicsBody(new btRigidBody(construction_info));
        m_dynamicsWorld->addRigidBody(entity->physicsBody());
    }
}

glm::vec2 State::getMousePosition() {
    sf::Vector2i windowCoords = sf::Mouse::getPosition(*Root().window);
    sf::Vector2f worldCoords = Root().window->mapPixelToCoords(windowCoords);
    return glm::vec2(worldCoords.x, worldCoords.y);
}

void State::drawEntities(sf::RenderTarget& target) {
    for(auto entity : m_entities) {
        entity->onDraw(target);
    }
}

void State::setView(sf::RenderTarget& target) {
    float w = m_zoom;
    float h = w / target.getSize().x * target.getSize().y;
    m_pixelSize = w / target.getSize().x;
    m_view.reset(sf::FloatRect(m_center.x-w/2, m_center.y-h/2, w, h));
    target.setView(m_view);
}
