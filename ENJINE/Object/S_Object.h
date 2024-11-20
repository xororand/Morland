#pragma once
#include <Component/Component.h>

class S_Object
{
public:
    enum Type {
        S_RootObj
    };

    virtual ~S_Object() = default;

    S_Object::Type getType()        { return m_type; }
    void setType(S_Object::Type t)  { m_type = t; }

    void addObject(S_Object* obj) { m_objects.push_back(obj); }

    virtual void onProcess() { };
    void process();
public:
    // ������������ ������
    S_Object*              m_parent;
    // �������� �������
    std::vector<S_Object*>  m_objects;
    // ���������� �������� �������
    std::vector<Component*> m_components;
private:
    // ��� ������
    S_Object::Type m_type;
};

