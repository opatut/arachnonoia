#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <btBulletDynamicsCommon.h>

#include "Entity.hpp"

class State {
public:
    State() = default;
    virtual ~State() = 0;

    void init();
    void update(double dt);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event& event);

    virtual void onInit() {}
    virtual void onUpdate(double dt) {}
    virtual void onDraw(sf::RenderTarget& target) {}
    virtual void onHandleEvent(sf::Event& event) {}

    void add(std::shared_ptr<Entity> entity);

protected:
    std::vector<std::shared_ptr<Entity>> m_entities;

    // physics stuff
    bool m_usePhysics;
    btBroadphaseInterface* m_broadphase = nullptr;
    btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
    btCollisionDispatcher* m_collisionDispatcher = nullptr;
    btSequentialImpulseConstraintSolver* m_solver = nullptr;
    btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
};

#endif
