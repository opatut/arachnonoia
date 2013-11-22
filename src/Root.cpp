#include "Root.hpp"

Root& Root::instance() {
    static Root _instance;
    return _instance;
}
