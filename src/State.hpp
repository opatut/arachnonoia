#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class State {
public:
    State();

    void update(double dt);
    void draw(sf::RenderTarget& target);

    virtual void onUpdate(double dt);
    virtual void onDraw(sf::RenderTarget& target);

    void add(std::shared_ptr<Entity> entity);

private:
    std::vector<std::shared_ptr<Entity>> m_entities;

};

#endif
