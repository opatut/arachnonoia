#ifndef PAIR_HPP
#define PAIR_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Entity.hpp"

#define PAIR_TYPE_MIN 1
#define PAIR_TYPE_MAX 4

class Pair : public Entity {
public:
    Pair();

    std::string getTypeName() const override;

    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onAdd(State* state);

    void setMetadata(int data);

    std::vector<std::shared_ptr<Pair>> findMatchingPairs();
    void deactivateAllOtherPairs();

    void setType(int type);
    void activate();
    void deactivate();
    void solve();

    sf::Color getColor() const;

    bool isActive() const;
    bool isSolved() const;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("entity", cereal::base_class<Entity>(this)));
        ar(cereal::make_nvp("type", m_type));
    }

private:
    int m_type;
    bool m_active;
    bool m_solved;
    float m_activationTime;
    float m_solvedTime;
    sf::Sound m_sound;
};

#endif
