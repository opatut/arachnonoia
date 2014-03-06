#include "State.hpp"

#include "Root.hpp"
#include "EntityMotionState.hpp"

#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
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
    m_dynamicsWorld->setGravity(btVector3(0, 9.81, 0));
}

void State::deinitializeWorld() {
    delete m_debugDrawer;
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_collisionDispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

void State::update(float dt) {
    m_time += dt;

    m_fpsTimer += dt;
    m_fpsCurrentCounter++;
    if(m_fpsTimer > 1.f) {
        m_fpsTimer -= 1.f;
        m_fps = m_fpsCurrentCounter;
        m_fpsCurrentCounter = 0;
    } else if(m_fps == -1) {
        m_fps = (int)(1 / dt);
    }

    m_dynamicsWorld->stepSimulation(dt, 10);

    m_total_elapsed += dt * 1000;
    m_tweener.step(m_total_elapsed);

    // remove deleted entities
    for(auto i = m_entities.begin(); i != m_entities.end(); ++i) {
        if((*i)->isDeleted()) {
            remove(*i);
            i++;
        }
    }

    onUpdate(dt);
    for(auto entity : m_entities) {
        entity->handleUpdate(dt);
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
                if (obA->getUserPointer() && obB->getUserPointer()) {
                    Entity* a = static_cast<Entity*>(obA->getUserPointer());
                    Entity* b = static_cast<Entity*>(obB->getUserPointer());

                    EntityCollision c;
                    c.distance = pt.getDistance();
                    c.other = b;
                    c.position = pt.getPositionWorldOnA();
                    c.otherPosition = pt.getPositionWorldOnB();
                    c.collisionObject = obA;
                    c.otherCollisionObject = obB;

                    if(!a->onCollide(b, c)) {
                        std::swap(c.position, c.otherPosition);
                        std::swap(c.collisionObject, c.otherCollisionObject);
                        c.other = a;
                        b->onCollide(a, c);
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

void State::onUpdate(float dt) {}

void State::onDraw(sf::RenderTarget& target) {
    drawEntities(target);
}

void State::onHandleEvent(sf::Event& event) {}

void State::add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
    initializeEntity(entity);
    entity->handleAddedToState(this);
}

void State::remove(std::shared_ptr<Entity> entity) {
    entity->onRemove(this);
    if(entity->physicsBody() != nullptr) {
        m_dynamicsWorld->removeRigidBody(entity->physicsBody());
    }
    m_entities.erase(std::find(m_entities.begin(), m_entities.end(), entity));
}

void State::initializeEntity(std::shared_ptr<Entity> entity) {
    entity->onInitialize();
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

glm::vec2 State::getMousePosition(bool local) {
    sf::Vector2i windowCoords = sf::Mouse::getPosition(*Root().window);
    if(local) {
        sf::Vector2f worldCoords = Root().window->mapPixelToCoords(windowCoords);
        return glm::vec2(worldCoords.x, worldCoords.y);
    } else {
        return glm::vec2(windowCoords.x, windowCoords.y);
    }
}

void State::drawEntities(sf::RenderTarget& target) {
    std::sort(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) -> bool { 
        if(a->zLevel() != b->zLevel()) {
            return a->zLevel() < b->zLevel(); 
        } else {
            return a->position().y < b->position().y;
        }
    });

    for(auto entity : m_entities) {
        entity->handleDraw(target);
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

    // cereal::JSONInputArchive ar(stream);
    if(filename.substr(filename.length() - 4) == "json") {
        cereal::JSONInputArchive ar(stream);
        ar(cereal::make_nvp("entities", m_entities));
    } else {
        cereal::PortableBinaryInputArchive ar(stream);
        ar(cereal::make_nvp("entities", m_entities));
    }
    stream.close();

    // reset the physics world
    deinitializeWorld();
    initializeWorld();
    for(auto entity: m_entities) {
        initializeEntity(entity);
        entity->handleAddedToState(this);
    }
}

void State::saveToFile(const std::string& filename) {
    std::ofstream stream;
    stream.open(filename);

    if(filename.substr(filename.length() - 4) == "json") {
        cereal::JSONOutputArchive ar(stream);
        ar(cereal::make_nvp("entities", m_entities));
    } else {
        cereal::PortableBinaryOutputArchive ar(stream);
        ar(cereal::make_nvp("entities", m_entities));
    }

    stream.close();
}

const std::vector<std::shared_ptr<Entity>>& State::getEntities() const {
    return m_entities;
}

std::map<Entity*, std::vector<EntityCollision>> State::getBodyContacts(btCollisionObject* from) {
    std::map<Entity*, std::vector<EntityCollision>> map;

    int numManifolds = dynamicsWorld()->getDispatcher()->getNumManifolds();
    for(int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold = dynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        if(obA != from && obB != from) continue;

        Entity* a = static_cast<Entity*>(obA->getUserPointer());
        Entity* b = static_cast<Entity*>(obB->getUserPointer());

        std::vector<EntityCollision> collisions;
        for (int j = 0; j < contactManifold->getNumContacts(); j++) {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);

            EntityCollision c;
            c.other = (obA == from) ? b : a;
            c.position = pt.getPositionWorldOnA();
            c.otherPosition = pt.getPositionWorldOnB();
            c.distance = pt.getDistance();
            c.collisionObject = obA;
            c.otherCollisionObject = obB;

            if(obA == from) {
                std::swap(c.position, c.otherPosition);
                std::swap(c.collisionObject, c.otherCollisionObject); 
            }

            collisions.push_back(c);
        }
        map[obA == from ? b : a] = collisions;
    }
    return map;
}

float State::getPixelSize() const {
    return m_pixelSize;
}

float State::getTime() const {
    return m_time;
}    

int State::getFPS() const {
    return m_fps;
}
