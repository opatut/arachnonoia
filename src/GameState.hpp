#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"
#include "Egg.hpp"
#include "Marker.hpp"

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
    void switchLevel(int num, bool reset = false);
    void nextLevel();

    void message(const std::string& msg);
    std::shared_ptr<Marker> getMarker(Marker::Type type);
    std::shared_ptr<Player> m_player;

private:
    int m_currentLevel;
    std::string m_currentLevelName;
    int m_nextLevel;
    std::string m_message;
    float m_messageTime = 0.f;

    std::shared_ptr<Egg> m_egg;
    sf::RenderTexture m_renderTextures[2];

    float m_levelFade;

    std::string m_currentHelp;
    float m_helpProgress = 0.f;
    std::vector<std::pair<std::string, Player::Ability>> m_levels;
    std::map<std::string, std::string> m_levelHelp;

    sf::Sound m_rumbleSound;
    std::shared_ptr<sf::Music> m_music;
};

#endif
