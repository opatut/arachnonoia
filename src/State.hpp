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

};

#endif
