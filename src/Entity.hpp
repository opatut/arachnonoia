#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = 0;

    virtual void onUpdate(double dt);
    virtual void onDraw(sf::RenderTarget& target);
    virtual void onHandleEvent(sf::Event& event);

    glm::vec2 position;
    float rotation;
};

#endif
