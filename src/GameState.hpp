#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class GameState : public State {
public:
    GameState();
    void init();

    void onUpdate(double dt);
    void onDraw(sf::RenderTarget& target);

private:
    std::shared_ptr<Player> m_player;

};

#endif
