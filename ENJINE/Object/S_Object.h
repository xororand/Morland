#pragma once
#include <Component/Component.h>

class S_Object
{
public:
    enum Type {
        S_RootObj
    };

    S_Object::Type getType()        { return m_type; }
    void setType(S_Object::Type t)  { m_type = t; }

    void addObject(std::shared_ptr<S_Object> obj) { m_objects.push_back(obj); }

    virtual ~S_Object() = default;
public:
    // �������� �������
    std::vector<std::shared_ptr<S_Object>>    m_objects;
    // ���������� �������� �������
    std::vector<std::shared_ptr<Component>> m_components;
private:
    // ��� ������
    S_Object::Type m_type;
};

