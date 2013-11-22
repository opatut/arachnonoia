#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class GameState : public State {
public:
    GameState();

    void onUpdate(double dt);
    void onDraw(sf::RenderTarget& target);

private:
    std::shared_ptr<Player> player;

};

#endif
