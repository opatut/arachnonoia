#ifndef COLLISIONSHAPE_HPP
#define COLLISIONSHAPE_HPP

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include "CerealGLM.hpp"

#include <SFML/Graphics.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Entity.hpp"

class CollisionShape : public Entity {
public:
    CollisionShape();
    std::string getTypeName() const override;

    // void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onInitialize();
    void onAdd(State *state) override;

    std::vector<std::vector<glm::vec2>>& shapes();

    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("entity", cereal::base_class<Entity>(this)));
        ar(cereal::make_nvp("shapes", m_shapes));
    }

private:
    std::vector<std::vector<glm::vec2>> m_shapes;
};

#endif
