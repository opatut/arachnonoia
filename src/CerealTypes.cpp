#include "Entity.hpp"
#include "Wall.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(Entity)
CEREAL_REGISTER_TYPE(Wall)

