#ifndef FOOT_HPP
#define FOOT_HPP

#include "Entity.hpp"
#include "State.hpp"

class Player;
class Foot : public Entity {
public:
    Foot(Player *player, int offset, bool background);

    std::string getTypeName() const override;

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget &target) override;

    // Directions:
    // -1 backward
    //  1 forward
    //  0 idle
    void setDirection(int direction);

private:
    Player* m_player;

    int m_offset;

    int m_direction = 0;
    float m_phase = 0;

    glm::vec2 m_anklePosition;
    bool m_background;
};

#endif // FOOT_HPP
