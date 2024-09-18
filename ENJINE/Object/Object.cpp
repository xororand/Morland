#include "Object.h"

Object::Type Object::getType() {
    return m_type;
}

void Object::setType(Object::Type t) {
    m_type = t;
}

void Object::addObject(std::shared_ptr<Object> obj)
{
    m_objects.push_back(obj);
}