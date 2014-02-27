#include "State.hpp"

#include "Root.hpp"
#include "EntityMotionState.hpp"

#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

void bulletTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    State* s = static_cast<State*>(world->getWorldUserInfo());
    s->worldTickCallback(timeStep);
}


State::~State() {
    deinitializeWorld();
}

void State::init() {
    initializeWorld();
    onInit();
}

void State::initializeWorld() {
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_debugDrawer = new DebugDraw();

    m_debugDrawer->setDebugMode(DebugDraw::DBG_DrawWireframe | DebugDraw::DBG_DrawContactPoints | DebugDraw::DBG_DrawConstraints | DebugDraw::DBG_DrawNormals);
    m_dynamicsWorld->setDebugDrawer(m_debugDrawer);

    m_dynamicsWorld->setWorldUserInfo(this);
    m_dynamicsWorld->setInternalTickCallback(bulletTickCallback, static_cast<void *>(this));
    m_dynamicsWorld->setGravity(btVector3(0, 3, 0));
}

void State::deinitializeWorld() {
    delete m_debugDrawer;
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_collisionDispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

void State::update(double dt) {
    m_dynamicsWorld->stepSimulation(dt, 10);

    onUpdate(dt);
    for(auto entity : m_entities) {
        entity->onUpdate(dt);
    }
}

void State::worldTickCallback(btScalar timestep) {
    int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
    for(int i=0;i<numManifolds;i++) {
        btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        int numContacts = contactManifold->getNumContacts();
        for (int j=0;j<numContacts;j++) {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if(pt.getLifeTime() == 1) {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;

                if (obA->getUserPointer() && obB->getUserPointer()) {
                    Entity* a = static_cast<Entity*>(obA->getUserPointer());
                    Entity* b = static_cast<Entity*>(obB->getUserPointer());

                    if(!a->onCollide(b)) {
                        b->onCollide(a);
                    }
                }
            }
        }
    }
}

void State::draw(sf::RenderTarget& target) {
    onDraw(target);

    setView(target);
    if(m_debugDrawEnabled) {
        m_dynamicsWorld->debugDrawWorld();
    }
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
    initializeEntity(entity);
    entity->m_state = this;
    entity->onAdd(this);
}

void State::remove(std::shared_ptr<Entity> entity) {
    entity->onRemove(this);
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
}

void State::initializeEntity(std::shared_ptr<Entity> entity) {
    // If there is no physics shape set, the entity probably doesn't like physics so leave it alone
    if(entity->physicsShape() != nullptr) {
        EntityMotionState* motionstate = new EntityMotionState(btTransform(btQuaternion(0, 0, entity->rotation()), btVector3(entity->position().x, entity->position().y, 0)), entity);
        entity->setMotionState(motionstate);
        btVector3 inertia(0, 0, 0);
        entity->physicsShape()->calculateLocalInertia(entity->mass(), inertia);
        btRigidBody::btRigidBodyConstructionInfo construction_info(entity->mass(), motionstate, entity->physicsShape(), inertia);
        entity->setPhysicsBody(new btRigidBody(construction_info));

        // We're in 2D land so don't allow Z movement
        entity->physicsBody()->setLinearFactor(btVector3(1, 1, 0));
        entity->physicsBody()->setAngularFactor(btVector3(0, 0, 1));

        // Store a pointer to the entity in there, maybe we'll need it
        entity->physicsBody()->setUserPointer((void*)entity.get());

        m_dynamicsWorld->addRigidBody(entity->physicsBody());
    }
}

glm::vec2 State::getMousePosition() {
    sf::Vector2i windowCoords = sf::Mouse::getPosition(*Root().window);
    sf::Vector2f worldCoords = Root().window->mapPixelToCoords(windowCoords);
    return glm::vec2(worldCoords.x, worldCoords.y);
}

void State::drawEntities(sf::RenderTarget& target) {
    std::sort(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) { return a->zLevel() - b->zLevel(); });

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

btDiscreteDynamicsWorld* State::dynamicsWorld() const {
    return m_dynamicsWorld;
}

void State::loadFromFile(const std::string& filename) {
    std::ifstream stream;
    stream.open(filename);

    cereal::JSONInputArchive ar(stream);
    ar(cereal::make_nvp("entities", m_entities));
    stream.close();

    // reset the physics world
    deinitializeWorld();
    initializeWorld();
    for(auto entity: m_entities) {
        initializeEntity(entity);
        entity->onAdd(this);
    }
}

void State::saveToFile(const std::string& filename) {
    std::ofstream stream;
    stream.open(filename);

    cereal::JSONOutputArchive ar(stream);
    ar(cereal::make_nvp("entities", m_entities));
    ar.finishNode();

    stream.close();
}
