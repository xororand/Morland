#pragma once
#include <Component/Component.h>
class WorldManager;
class S_Object
{
public:
    enum Type {
        S_RootObj,
        S_PlayerObj
    };

    virtual ~S_Object() = default;

    WorldManager* getWM()           { return m_wm; }
    void setWM(WorldManager* wm)    { this->m_wm = wm; }

    S_Object::Type getType()        { return m_type; }
    void setType(S_Object::Type t)  { m_type = t; }

    void addObject(S_Object* obj) { m_objects.push_back(obj); }

    virtual void onProcess() { };
    void process();
public:
    // ������������ ������
    S_Object*               m_parent = nullptr;
    // �������� �������
    std::vector<S_Object*>  m_objects;
    // ���������� �������� �������
    std::vector<Component*> m_components;
private:
    WorldManager* m_wm;
    // ��� ������
    S_Object::Type m_type;
};

