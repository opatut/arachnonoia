#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Entity.hpp"
#include "Foot.hpp"

class Player : public Entity {
public:
    Player();

    std::string getTypeName() const override;

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State *state) override;
    bool onCollide(Entity* other, const EntityCollision& c) override;
    void onHandleEvent(sf::Event& event) override;

    void jump();
    int getPairsLeft() const;

    enum Ability {
        WALK = 0,
        JUMP = 1,
        WALLS = 2,
        RAPPEL = 3
    };

    void setAbility(Ability ability);
    Ability getAbility() const;

private:
    sf::Sprite m_sprite;
    btGhostObject* m_ghostObject;
    float m_scale_y = 0;
    float m_springPower = 0;
    std::vector<std::shared_ptr<Foot>> m_foregroundFeet;

    Ability m_ability;
};

#endif
