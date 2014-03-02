#ifndef MARKER_HPP
#define MARKER_HPP

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include "CerealGLM.hpp"

#include <SFML/Graphics.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Entity.hpp"

class Marker : public Entity {
public:
    enum Type {
        NONE = 0,
        SPAWN = 1,
        GOAL = 2
    };

public:
    Marker();
    std::string getTypeName();

    void onInitialize() override;
    void onAdd(State* state) override;
    void onDraw(sf::RenderTarget& target) override;

    void setMetadata(int data) override;

    glm::vec2 getSize() override;

    Type getType() const;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("entity", cereal::base_class<Entity>(this)));
        ar(cereal::make_nvp("type", m_type));
    }

private:
    Type m_type;
};

#endif
