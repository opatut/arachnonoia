#ifndef ROOT_HPP
#define ROOT_HPP

#include "ResourceManager.hpp"

class Root {
public:
    // objects
    ResourceManager resources;

    static Root& instance();
};

#endif
