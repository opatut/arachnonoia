#ifndef WALL_HPP
#define WALL_HPP

#include <SFML/Graphics.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#include "Entity.hpp"

class Wall : public Entity {
public:
    static const std::string types[3];
    Wall();
    
    std::string getTypeName();

    void onUpdate(double dt) override;
    void onDraw(State *state, sf::RenderTarget& target) override;
    void onAdd(State* state);

    void setMetadata(int data);
    void setType(const std::string& type);

    glm::vec2 getSize();

    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("entity", cereal::base_class<Entity>(this)));
        ar(cereal::make_nvp("m_type", m_type));
    }

private:
    std::string m_type;
    sf::Sprite m_sprite;
};

#endif
