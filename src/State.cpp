#include "State.hpp"

#include "EntityMotionState.hpp"

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

    // if there is no physics shape set, the entity probably doesn't like physics so leave it alone
    if(entity->m_physicsShape != nullptr && m_usePhysics) {
        EntityMotionState* motionstate = new EntityMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)), entity);
        btScalar mass = 1;
        btVector3 inertia(0, 0, 0);
        entity->m_physicsShape->calculateLocalInertia(mass, inertia);
        btRigidBody::btRigidBodyConstructionInfo construction_info(mass, motionstate, entity->m_physicsShape, inertia);
        entity->m_physicsBody = new btRigidBody(construction_info);
        m_dynamicsWorld->addRigidBody(entity->m_physicsBody);
    }
}
