#ifndef FOOT_HPP
#define FOOT_HPP

#include "Entity.hpp"
#include "State.hpp"

class Foot : public Entity {
public:
    Foot(Entity* player, int offset);

    std::string getTypeName() const;

    void onUpdate(float dt);
    void onDraw(sf::RenderTarget &target);

    // Directions:
    // -1 backward
    //  1 forward
    //  0 idle
    void setDirection(int direction);

    glm::vec2 m_anklePosition;

private:
    Entity* m_player;

    int m_offset;

    int m_direction = 0;
    float m_phase = 0;
    bool m_reverse = false;
};

#endif // FOOT_HPP
