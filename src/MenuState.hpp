#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <memory>

#include "State.hpp"

class MenuState : public State {
public:
    MenuState();

    void onUpdate(float dt) override;
    void onDraw(sf::RenderTarget& target) override;
};

#endif
