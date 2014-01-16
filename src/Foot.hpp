#ifndef FOOT_HPP
#define FOOT_HPP

#include "Entity.hpp"

class Foot : public Entity {
public:
    Foot();

    void onDraw(sf::RenderTarget &target);
    void onAdd(State* state);
};

#endif // FOOT_HPP
