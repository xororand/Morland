#include "SpriteObj.h"

void SpriteObj::draw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	sf::Transform combinedTransform = transform * getTransform();

	onDraw(target, combinedTransform);

	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (m_objects[i] == NULL) continue;
		std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object>(m_objects[i]);
		Object::Type objtype = obj.get()->getType();

		// ������ ������ ������� ����� ���� ���������
		if (objtype == Type::SpriteObject) {
			std::shared_ptr<SpriteObj> spriteObj = std::dynamic_pointer_cast<SpriteObj>(obj);

			//combinedTransform = transform * spriteObj.get()->getTransform();
			spriteObj.get()->draw(target, combinedTransform);
		}
	}
		
}

void SpriteObj::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(m_sprite, transform);
}
