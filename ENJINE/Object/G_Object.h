#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <Component/Component.h>

using namespace sf;

class G_Object : public Transformable {
public:
    enum Type {
        RootObj,
        SpriteObject
    };

    G_Object::Type getType()                          { return m_type; }
    void setType(G_Object::Type t)                    { m_type = t; }

    void addObject(std::shared_ptr<G_Object> obj)     { m_objects.push_back(obj); }

	virtual ~G_Object() = default;
public:
    // Дочерние объекты
    std::vector<std::shared_ptr<G_Object>>      m_objects;
    // Компоненты текущего объекта
    std::vector<std::shared_ptr<Component>>     m_components;
private:
    // Тип объект
    G_Object::Type m_type;
};

