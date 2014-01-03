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

    virtual void onInit();
    virtual void onUpdate(double dt);
    virtual void onDraw(sf::RenderTarget& target);
    virtual void onHandleEvent(sf::Event& event);

    void add(std::shared_ptr<Entity> entity);

    glm::vec2 getMousePosition();

protected:
    void drawEntities(sf::RenderTarget& target);
    void setView(sf::RenderTarget& target);

    std::vector<std::shared_ptr<Entity>> m_entities;

    float m_zoom;
    glm::vec2 m_center;
    sf::View m_view;
    float m_pixelSize;

    // physics stuff
    bool m_usePhysics;
    btBroadphaseInterface* m_broadphase = nullptr;
    btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
    btCollisionDispatcher* m_collisionDispatcher = nullptr;
    btSequentialImpulseConstraintSolver* m_solver = nullptr;
    btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
};

#endif
