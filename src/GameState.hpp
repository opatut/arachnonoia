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

private:
    std::shared_ptr<Player> m_player;

};

#endif
