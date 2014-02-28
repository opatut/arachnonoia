#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <btBulletDynamicsCommon.h>

#include <CppTweener.h>

#include "Entity.hpp"
#include "DebugDraw.hpp"

class State {
public:
    State() = default;
    virtual ~State() = 0;

    void init();
    void initializeWorld();
    void deinitializeWorld();

    void update(float dt);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event& event);
    void worldTickCallback(btScalar timestep);

    virtual void onInit();
    virtual void onUpdate(float dt);
    virtual void onDraw(sf::RenderTarget& target);
    virtual void onHandleEvent(sf::Event& event);

    void add(std::shared_ptr<Entity> entity);
    void remove(std::shared_ptr<Entity> entity);
    void initializeEntity(std::shared_ptr<Entity> entity);

    glm::vec2 getMousePosition(bool local = true);

    btDiscreteDynamicsWorld* dynamicsWorld() const;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    tween::Tweener m_tweener;
    sf::Uint64 m_total_elapsed = 0;

    const std::vector<std::shared_ptr<Entity>>& getEntities() const;
    
    template<typename T>
    std::vector<std::shared_ptr<T>> getEntitiesByType(const std::string& typeName) const {
        std::vector<std::shared_ptr<T>> r;
        for(auto e : m_entities) {
            if(e->getTypeName() == typeName) {
                r.push_back(std::static_pointer_cast<T>(e));
            }
        }
        return r;
    }

    std::map<Entity*, std::vector<EntityCollision>> getBodyContacts(btCollisionObject* from);

    float getPixelSize() const;

protected:
    void drawEntities(sf::RenderTarget& target);
    void setView(sf::RenderTarget& target);

    std::vector<std::shared_ptr<Entity>> m_entities;

    float m_zoom;
    glm::vec2 m_center;
    sf::View m_view;
    float m_pixelSize;

    // physics stuff
    btBroadphaseInterface* m_broadphase = nullptr;
    btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
    btCollisionDispatcher* m_collisionDispatcher = nullptr;
    btSequentialImpulseConstraintSolver* m_solver = nullptr;
    btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
    DebugDraw* m_debugDrawer = nullptr;
    bool m_debugDrawEnabled = false;
};

#endif
