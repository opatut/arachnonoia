#ifndef EGG_HPP
#define EGG_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Entity.hpp"

class Egg : public Entity {
public:
    enum Type {
        FULL,
        UPPER,
        LOWER
    };

    Egg(Type type = FULL);
    std::string getTypeName() const override;

    void onInitialize() override;
    void onAdd(State* state) override;
    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onHandleEvent(sf::Event& event) override;

    void setHatching(bool hatching);

private:
    bool m_hatching;
    float m_progress;
    Type m_type;
    sf::Sound m_sound;
};

#endif
