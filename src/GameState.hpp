#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"
#include "Egg.hpp"

#define LEVEL_COUNT 2

class GameState : public State {
public:
    GameState();

    void onInit() override;
    void onUpdate(float dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onHandleEvent(sf::Event& event) override;

    void resize();

    void loadLevel(int num);
    void spawnPlayer(const glm::vec2& pos);
    void spawnEgg(const glm::vec2& pos);
    void switchLevel(int num);
    void nextLevel();

    void message(const std::string& msg);

private:
    int m_currentLevel;
    int m_nextLevel;
    std::string m_message;
    float m_messageTime = 0.f;

    std::shared_ptr<Player> m_player;
    std::shared_ptr<Egg> m_egg;
    sf::RenderTexture m_renderTextures[2];

    float m_levelFade;
};

#endif
