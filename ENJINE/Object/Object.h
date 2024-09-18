#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

using namespace sf;

class Object : public Transformable {
public:
    enum Type {
        SpriteObject
    };

    Object::Type getType();
    void setType(Object::Type t);

    void addObject(std::shared_ptr<Object> obj);

	virtual ~Object() = default;
public:
    // Дочерние объекты
    std::vector<std::shared_ptr<Object>> m_objects;
    // Тип объекта
private:
    Object::Type m_type;
};

