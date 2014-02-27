#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class GameState : public State {
public:
    GameState();

    void onInit() override;
    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onHandleEvent(sf::Event& event) override;

    void resize();

public:
    std::shared_ptr<Player> m_player;
    sf::RenderTexture m_renderTexture;

};

#endif
