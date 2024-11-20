#include "S_Object.h"

// Process object
void S_Object::process()
{
	// Вызов самого объекта
	onProcess();
	// Вызываем обработку дочерних
	for (auto obj : m_objects) obj->process();
}