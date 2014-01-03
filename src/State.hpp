#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class State {
public:
    State() = default;

    virtual void init() = 0;

    void update(double dt);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event& event);

    virtual void onUpdate(double dt) {}
    virtual void onDraw(sf::RenderTarget& target) {}
    virtual void onHandleEvent(sf::Event& event) {}

    void add(std::shared_ptr<Entity> entity);

private:
    std::vector<std::shared_ptr<Entity>> m_entities;

};

#endif
