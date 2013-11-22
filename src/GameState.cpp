#include "GameState.hpp"

#include <iostream>

GameState::GameState() {
    player = std::make_shared<Player>();
    add(player);
}

void GameState::onUpdate(double dt) {

}

void GameState::onDraw(sf::RenderTarget& target) {
}
