#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include <Component/Component.h>

using namespace sf;

class Object : public Transformable {
public:
    enum Type {
        SpriteObject
    };

    Object::Type getType()                          { return m_type; }
    void setType(Object::Type t)                    { m_type = t; }

    void addObject(std::shared_ptr<Object> obj)     { m_objects.push_back(obj); }

	virtual ~Object() = default;
public:
    // �������� �������
    std::vector<std::shared_ptr<Object>>    m_objects;
    // ���������� �������� �������
    std::vector<std::shared_ptr<Component>> m_components;
private:
    // ��� ������
    Object::Type m_type;
};

