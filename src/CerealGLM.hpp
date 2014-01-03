#ifndef CEREAL_GLM
#define CEREAL_GLM

#include <cereal/cereal.hpp>
#include <glm/glm.hpp>

namespace cereal {

template<class Archive>
void serialize(Archive& ar, glm::vec2& v) {
    ar(cereal::make_nvp("x", v.x));
    ar(cereal::make_nvp("y", v.y));
}

}

#endif
