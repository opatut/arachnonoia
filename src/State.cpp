#include "State.hpp"

void State::init() {
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -1, 0));

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
}

void State::draw(sf::RenderTarget& target) {
    onDraw(target);
    for(auto entity : m_entities) {
        entity->onDraw(target);
    }
}

void State::handleEvent(sf::Event& event) {
    onHandleEvent(event);
    for(auto entity : m_entities) {
        entity->onHandleEvent(event);
    }
}

void State::add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
}
