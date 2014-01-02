#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <cereal/cereal.hpp>

class Level {
public:
    float width, height;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(CEREAL_NVP(width));
        archive(CEREAL_NVP(height));
    }

    void load(const std::string& filename);
};

#endif
