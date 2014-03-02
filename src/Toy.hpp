#ifndef TOY_HPP
#define TOY_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Toy : public Entity {
public:
    Toy();
    std::string getTypeName();

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State *state);

private:
};

#endif
