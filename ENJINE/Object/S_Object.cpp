#include "S_Object.h"

// Process object
void S_Object::process()
{
	// ����� ������ �������
	onProcess();
	// �������� ��������� ��������
	for (auto obj : m_objects) obj->process();
}