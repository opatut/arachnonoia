#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class GameState : public State {
public:
    GameState();

    void onInit() override;
    void onUpdate(float dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onHandleEvent(sf::Event& event) override;

    void resize();

    std::shared_ptr<Player> m_player;
    sf::RenderTexture m_renderTexture;

    enum m_CollisionTypes {
        COL_NOTHING = 0,
        COL_WALL = 1 << 1,
        COL_PLAYER = 1 << 2
    };
};

#endif
