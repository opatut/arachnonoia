#ifndef PAIR_HPP
#define PAIR_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Pair : public Entity {
public:
    Pair();

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State* state);

    void setGlyphNumber(int number);

    static std::string getGlyphName(int number);

private:
    int m_glyphNumber;
    bool m_active;
    float m_activationTime;

    sf::Sprite m_sprite;
    sf::Sprite m_glyphSprite;
};

#endif
