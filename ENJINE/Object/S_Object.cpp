#include "S_Object.h"

// Process object
void S_Object::process()
{
	// ����� ������ �������
	onProcess();
	// �������� ��������� ��������
	for (auto obj : m_objects) obj->process();

	//for (auto obj : m_objects) {
	//	// ������� ���� ������ �������
	//	if (obj == nullptr) continue;
	//	obj->process();
	//}
	//// ������� ������� ������ �� �������
	//for (std::vector<S_Object*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	//	if (it._Ptr == nullptr) m_objects.erase(it);
}