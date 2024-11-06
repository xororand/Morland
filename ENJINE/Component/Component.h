#pragma once
#include "SFML/Network.hpp"
#include "mGame/Managers/Network/defines.h"

using namespace sf;

class Component {
public:
    enum Type {
        None,
        HealthComponent
    };

    Component::Type getType()           { return m_type;    }
    void setType(Component::Type t)     { m_type = t;       }

    virtual ~Component() = default;
private:
    Component::Type m_type = None;
};



