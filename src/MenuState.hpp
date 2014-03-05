#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <memory>

#include "State.hpp"

class MenuState : public State {
public:
    MenuState();

    void onInit() override;
    void onUpdate(float dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onHandleEvent(sf::Event& event) override;

    void resize();

private:
    sf::RenderTexture m_renderTextures[2];        
};

#endif
